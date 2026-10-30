#!/usr/bin/env python3
"""Bounded gameplay discovery through the real SDL player; no VM/state injection."""
import argparse
from collections import deque
from dataclasses import asdict, dataclass, field
import hashlib
import html
import struct
import zlib
import json
import os
from pathlib import Path
import re
import random
import shutil
import signal
import subprocess
import sys
import time

from gameplay_tests import PORT, DURABLE, digest, prepare
from explore_policy import MixedPolicy
from settings_checks import validate_profile,check_settings,verify_restart,check_flow
from behavior_checks import transition_checks, media_evidence, check_assertion

FINGERPRINT_VERSION = 6
PROGRESS_VERSION = 3
STATE_FIELDS = ('scene', 'background', 'layer', 'media_disc', 'session_ended',
                'effective_pool', 'variables', 'inventory', 'channels', 'channel',
                'selected', 'objects', 'active_slots', 'enabled', 'conversation', 'sounds',
                'random_state', 'morris', 'inventory_ui', 'menu', 'settings')
SAVE_FIELDS = tuple('effective_pool' if k=='variable_pool' else k for k in DURABLE)
REPLAY_FIELDS = SAVE_FIELDS + ('background', 'active_slots', 'media_disc')
PRIORITY = {'hotspot': 0, 'examine': 1, 'camera': 2, 'inventory': 3, 'wait': 4,'settings':5}


def write_json(path, value):
    path = Path(path)
    temporary = path.with_suffix(path.suffix + '.tmp')
    temporary.write_text(json.dumps(value, indent=2, ensure_ascii=False) + '\n')
    temporary.replace(path)


def preview_ppm(source, destination):
    from gameplay_tests import ppm
    raw=ppm(source)
    rows=b''.join(b'\0'+raw[y*640*3:(y+1)*640*3] for y in range(480))
    def chunk(kind,data):
        return struct.pack('>I',len(data))+kind+data+struct.pack('>I',zlib.crc32(kind+data)&0xffffffff)
    destination.write_bytes(b'\x89PNG\r\n\x1a\n'+chunk(b'IHDR',struct.pack('>IIBBBBB',640,480,8,2,0,0,0))+
                            chunk(b'IDAT',zlib.compress(rows))+chunk(b'IEND',b''))


def ensure_preview(source):
    destination=source.with_suffix('.png')
    try:
        if not destination.is_file():preview_ppm(source,destination)
        return destination
    except (AssertionError,OSError,ValueError):
        # A crash/timeout may interrupt a screenshot write. Keep the finding.
        return None


def render_report(output, report):
    esc=lambda value:html.escape(str(value),quote=True)
    cards=[]
    for item in report['findings']:
        folder=Path(item['bundle']) if item.get('bundle') else None
        link=os.path.relpath(folder,output) if folder else ''
        images=[]
        if folder and (folder/'checkpoints').is_dir():
            for name in ('saved','loaded','changed','replayed'):
                source=folder/'checkpoints'/(name+'.ppm')
                if source.is_file():
                    destination=ensure_preview(source)
                    if destination is None:continue
                    images.append(f'<figure><figcaption>{esc(name)}</figcaption><img src="{esc(link)}/checkpoints/{name}.png"></figure>')
            if not images:
                files=sorted((folder/'checkpoints').glob('p*.ppm'))
                if files:
                    destination=ensure_preview(files[-1])
                    if destination:images.append(f'<img src="{esc(link)}/checkpoints/{esc(destination.name)}">')
        cards.append(f'<article><h3>{esc(item.get("id",item["kind"]))}: {esc(item["kind"])}</h3>'
                     f'<p>{esc(item["reason"])} · {esc(item["scene"])} · {esc(item["severity"])} · '
                     f'Tekrarlandı: {esc(item.get("reproduced",False))}</p>'
                     +(f'<p><a href="{esc(link)}/replay.json">Tekrar çalıştırma dosyası</a> · '
                       f'<a href="{esc(link)}/player.log">Günlük</a></p>' if folder else '')+
                     '<div class="images">'+''.join(images)+'</div></article>')
    rows=''.join(f'<tr><td>{esc(s["seed"])}</td><td>{esc(s["scene"])}</td><td>{esc(s["background"])}</td>'
                 f'<td>{s["depth"]}</td><td><a href="{esc(os.path.relpath(s["trial"],output))}/actions.txt">Giriş dizisi</a></td></tr>'
                 for s in report['states'])
    page="""<!doctype html><html lang="tr"><meta charset="utf-8"><title>Otomatik oyun keşfi</title>
<style>body{font:16px system-ui;max-width:1100px;margin:32px auto;padding:0 20px;background:#181b20;color:#eee}a{color:#e7bf68}article{background:#282c33;padding:16px;margin:18px 0;border-radius:8px}table{border-collapse:collapse;width:100%}td,th{padding:8px;text-align:left;border-bottom:1px solid #555}.images{display:flex;gap:12px;flex-wrap:wrap}figure{margin:0;max-width:48%}img{max-width:100%;width:480px}code{overflow-wrap:anywhere}</style>
"""
    page+=f'<h1>Otomatik oyun keşfi: {esc(report["status"])}</h1>'
    page+=f'<p>{report.get("runs",0)} çalıştırma · {len(report["states"])} ayrıntılı durum · {len(report.get("progress_states",[]))} ilerleme/bağlam durumu · {len(report["findings"])} bulgu · sınır: {esc(report.get("stop_reason","running"))}</p>'
    search=report.get('search',{})
    if search:page+=f'<p>Tarama: {esc(search.get("strategy"))} · rastgelelik tohumu: {esc(search.get("random_seed"))} · çalıştırma üst sınırı: {esc(search.get("max_runs"))}</p>'
    metrics=report.get('policy_metrics')
    if metrics:page+=f'<p>En uzun yol: {metrics["max_depth"]} adım · seçim dağılımı: {esc(metrics["lanes"])}</p>'
    page+='<p>Bu rapor sınırlı bir taramadır; oyunun tamamlandığını kanıtlamaz. İnceleme adayları kesin oyun hatası sayılmaz.</p>'
    page+=('<p>Medya açık; SDL sanal cihazları kullanılıyor.</p>' if report.get('with_media') else '<p>Ses/video kapalı: mantık ve giriş akışı taraması.</p>')
    page+='<p><a href="report.json">Makine tarafından okunabilir rapor</a></p>'
    pool=report.get('seed_pool')
    if pool:page+=f'<p>Başlangıç havuzu: {pool["seed_count"]} kayıt · {len(pool["scenes"])} sahne. <a href="seed-pool.json">Başlangıçlar</a></p>'
    page+=f'<p>Son adımda ilerleme/bağlam değişimi: {report.get("progress_changes",0)} · yalnız ayrıntılı durum değişimi: {report.get("interaction_only_changes",0)}</p>'
    page+='<p>İlerleme/bağlam sayacı temkinli bir göstergedir; çözülen bulmaca sayısı değildir. Bilinmeyen değişkenler ve sahneye özgü kapılar görünüm değişikliklerini de sayabilir. Ayrıntılı durumlar ayrıca taranır.</p>'
    page+='<h2>Bulgular</h2>'+(''.join(cards) or '<p>Bu taramada bulgu yok.</p>')
    page+='<h2>Keşfedilen durumlar</h2><table><tr><th>Başlangıç</th><th>Sahne</th><th>Görüntü</th><th>Adım</th><th>Kanıt</th></tr>'+rows+'</table></html>'
    (output/'index.html').write_text(page)


def fingerprint(state):
    """A documented search abstraction, not equality of every VM/RAM byte."""
    missing = [k for k in STATE_FIELDS if k not in state]
    if missing or state.get('exploration', {}).get('schema') != 1:
        raise ValueError('missing exploration state fields: ' + ','.join(missing))
    if 'record_callbacks' not in state['exploration']:
        raise ValueError('missing exploration record callback state')
    value = {k: state[k] for k in STATE_FIELDS}
    camera = state.get('panorama')
    value['panorama'] = ([round(camera[0] % 360, 2) % 360, round(camera[1], 2)]
                         if camera else None)
    value['record_identity'] = state['exploration']['record_identity']
    value['record_callbacks'] = state['exploration']['record_callbacks']
    value['hover_preview'] = state['exploration']['hover_preview']
    value['return_context'] = state['exploration'].get('return_context')
    return hashlib.sha256(json.dumps(value, sort_keys=True, separators=(',', ':')).encode()).hexdigest()


def progress_projection(state):
    """Conservative progress/context signal; never used to prune detailed states.

    Unknown VM variables and dialogue fields are retained rather than guessed
    away. Scene-local gates can also reflect a view change, not a solved puzzle.
    Inventory states 1/3/4 mean ordinary/new/being examined in inventory.cpp;
    possession comes from channel membership, not the selected entry or loading.
    """
    fingerprint(state)  # Fail closed on incomplete observations.
    return {
        'scene': state['scene'], 'session_ended': state['session_ended'],
        'effective_pool': state['effective_pool'],
        'inventory': {key: (1 if value[0] in (1, 3, 4) else value[0])
                      for key, value in state['inventory'].items()},
        'channels': [sorted(set(channel)) for channel in state['channels']],
        'conversation': state['conversation'],
        'object_gates': {key: value[4:] for key, value in state['objects'].items()},
        'enabled': state['enabled'], 'morris': state['morris'],
        'record_continuations': {key: [value[2],value[3]]
                                 for key,value in state['exploration']['record_callbacks'].items()},
    }


def progress_fingerprint(state):
    return hashlib.sha256(json.dumps(progress_projection(state), sort_keys=True,
                          separators=(',', ':')).encode()).hexdigest()


def persistence_projection(state):
    # The second inventory element is a lazy asset-cache flag, not possession.
    return dict(state, inventory={key: value[0] for key, value in state['inventory'].items()})


def differences(left, right, fields):
    return [key for key in fields if key not in left or key not in right or left[key] != right[key]]


@dataclass
class Action:
    id: str
    category: str
    commands: list

    @classmethod
    def read(cls, value):
        if set(value) != {'id', 'category', 'commands'}:
            raise ValueError('invalid action schema')
        action = cls(**value)
        if not re.fullmatch(r'[A-Za-z0-9_-]+', action.id) or action.category not in PRIORITY:
            raise ValueError('invalid action identifier/category')
        if not isinstance(action.commands, list) or not 1 <= len(action.commands) <= (32 if action.category=='settings' else 12):
            raise ValueError('invalid action command count')
        for line in action.commands:
            parts = line.split()
            if len(parts) == 3 and parts[0] == 'click':
                if not (0 <= int(parts[1]) < 640 and 0 <= int(parts[2]) < 480):
                    raise ValueError('click outside logical screen')
            elif len(parts) == 2 and parts[0] == 'key' and (parts[1] in ('Left','Right','Up','Down') or
                    (action.category=='settings' and (parts[1] in ('Tab','Return','Escape') or re.fullmatch(r'F(?:[1-9]|1[0-9]|2[0-4])',parts[1])))):
                pass
            elif action.category=='settings' and len(parts)==3 and parts[0]=='resize' and 320<=int(parts[1])<=2560 and 240<=int(parts[2])<=1920:
                pass
            elif action.category=='settings' and len(parts)==2 and parts[0]=='focus' and parts[1] in ('lost','gained'):
                pass
            elif action.category=='settings' and len(parts)==3 and parts[0]=='hover' and 0<=int(parts[1])<640 and 0<=int(parts[2])<480:
                pass
            elif len(parts) == 2 and parts[0] == 'wait' and 1 <= int(parts[1]) <= 5000:
                pass
            else:
                raise ValueError('unsupported discovery input: ' + line)
        return action


@dataclass
class Plan:
    seed: str
    actions: list = field(default_factory=list)
    choices: list = field(default_factory=list)
    expected: list = field(default_factory=list)

    progress_priority: bool = False
    origin_progress: str = ''
    origin_scene: str = ''
    goal: str = ''

    def key(self):
        return json.dumps([self.seed, [asdict(a) for a in self.actions], self.choices], sort_keys=True)


@dataclass
class Issue:
    kind: str
    severity: str
    reason: str
    scene: str = ''

    def key(self):
        return self.kind, self.scene, self.reason


@dataclass
class Trial:
    directory: Path
    plan: Plan
    states: list
    action_steps: dict
    issue: object = None
    exit_code: object = None
    validation: dict = field(default_factory=dict)

    @property
    def final(self):
        return self.states[-1] if self.states else None


def execute(command, directory, timeout):
    env = {key: value for key, value in os.environ.items() if not key.startswith('BYZ_')}
    env.update(SDL_VIDEODRIVER='dummy', SDL_AUDIODRIVER='dummy', BYZ_LOG_ACTIVE='1')
    with (directory/'player.log').open('w') as log:
        process = subprocess.Popen(command, cwd=PORT, env=env, stdout=log,
                                   stderr=subprocess.STDOUT, start_new_session=os.name == 'posix')
        try:
            return process.wait(timeout=timeout), False
        except (subprocess.TimeoutExpired, KeyboardInterrupt) as error:
            try:
                if os.name == 'posix':os.killpg(process.pid, signal.SIGKILL)
                else:process.kill()
            except ProcessLookupError:
                pass
            process.wait()
            if isinstance(error,KeyboardInterrupt):raise
            return process.returncode, True


def classify(log, code, timed_out, states):
    scene = states[-1]['scene'] if states else ''
    if 'exploration decision budget exhausted' in log:
        return Issue('decision_budget', 'review', '128 automatic answer decisions reached', scene)
    if timed_out:
        return Issue('timeout', 'review', 'player exceeded trial timeout', scene)
    if code:
        last = next((line for line in reversed(log.splitlines()) if 'error' in line.lower()
                     or 'stopped:' in line or 'what()' in line), f'player exit {code}')
        return Issue('player_failure', 'error', last, scene)
    # VM probes are evaluations, not proof of a failed committed action.
    blocked = re.search(r'^click at .* stopped: (.+)', log, re.MULTILINE)
    if blocked:
        return Issue('blocked_action', 'error', blocked.group(1), scene)
    if re.search(r'choice=\d+ outside the \d+ answers',log):
        return Issue('stale_choice','review','answer prefix no longer matches available list',scene)
    if 'click_not_committed' in log or 'conversation stopped before completion' in log:
        return Issue('action_not_committed', 'error', 'action execution refused completion', scene)
    if 'automation_complete actions=' not in log:
        return Issue('incomplete_script', 'error', 'script did not finish', scene)
    if states and states[-1]['exploration']['blocker']:
        return Issue('runtime_blocker', 'error', states[-1]['exploration']['blocker'], scene)
    return None


def action_candidates(state, previous=None):
    if state['session_ended'] or state['menu'] or state['settings']:
        return []
    actions = [Action.read(a) for a in state['exploration']['candidates']]
    # A second unchanged wait does not expand the horizon indefinitely.
    if previous and previous.category == 'wait':
        actions = [a for a in actions if a.category != 'wait']
    return sorted(actions, key=lambda a: (PRIORITY[a.category], a.id))


def candidate_goal(action,state):
    # Kind0x17 is the existing map destination/highlight rule. This is
    # scheduling intent only, not proof that a click actually travelled.
    if action.category=='hotspot' and action.id.startswith('hotspot_'):
        slot=action.id[len('hotspot_'):]
        record=state['objects'].get(slot)
        if record and len(record)>5 and record[5]==0x17:return 'travel'
    return action.category


def decision_branches(trial, max_decisions):
    if not trial.final:
        return []
    decisions = trial.final['exploration']['decisions']
    branches = []
    actual = []
    for index, decision in enumerate(decisions[:max_decisions]):
        step = trial.action_steps.get(decision['action'], 0)
        for choice in range(len(decision['lines'])):
            if choice != decision['choice']:
                # Stop at the input that opened this answer list. Do not replay
                # stale descendant clicks into a different conversation branch.
                branches.append(Plan(trial.plan.seed, trial.plan.actions[:step],
                                     actual + [choice], trial.plan.expected[:step],origin_scene=decision.get('scene',trial.final.get('scene','')),goal='dialogue'))
        actual.append(decision['choice'])
    return branches


def wait_issue(state, previous):
    if (previous and previous.category=='wait' and not state['session_ended']
            and not state['menu'] and not state['settings'] and not state['active_slots']
            and not state['exploration']['script_end'] and not state['exploration']['scene_switch_pending']
            and not any(a.category!='wait' for a in action_candidates(state))):
        return Issue('no_actions','review','no available player action after wait',state['scene'])
    return None


class BudgetReached(Exception):
    pass


class Runner:
    def __init__(self, args, output, specs):
        self.args, self.output = args, output
        self.started = time.monotonic()
        self.deadline = self.started + args.max_seconds
        self.runs = 0
        self.seeds = {}
        self.engine, self.tables = prepare(args.data, output)
        self.assets = output/'assets'
        self.assets.mkdir()
        excluded = {'copy2hd','disc-media','disc-files','video-index','dialog_cache','.cache'}
        for child in args.data.iterdir():
            if child.name not in excluded:
                (self.assets/child.name).symlink_to(child.resolve(), target_is_directory=child.is_dir())
        (self.assets/'copy2hd').mkdir()
        for child in (args.data/'copy2hd').iterdir():
            if child.name.lower() != 'savegame':
                (self.assets/'copy2hd'/child.name).symlink_to(child.resolve(),target_is_directory=child.is_dir())
        (output/'inputs').mkdir()
        for spec in specs:
            name, scene, save = spec['id'], spec['scene'], spec['save']
            if (not re.fullmatch(r'[A-Za-z0-9_-]+', name) or name in self.seeds
                    or not re.fullmatch(r'[A-Za-z0-9_]+',scene)
                    or Path(save).name != save or not save.lower().endswith('.btb')
                    or type(spec['label']) is not int or not 0 <= spec['label'] <= 65535):
                raise ValueError('invalid or duplicate seed')
            if type(spec.get('entry',False)) is not bool:raise ValueError('seed entry must be boolean')
            if type(spec.get('restore',False)) is not bool:raise ValueError('seed restore must be boolean')
            if spec.get('restore') and spec.get('entry'):raise ValueError('restore and entry are mutually exclusive')
            for action in spec.get('warmup',[]):Action.read(action)
            folder = output/'inputs'/name
            folder.mkdir()
            source = Path(spec.get('save_source', args.data/'copy2hd/savegame'/save))
            shutil.copy2(source, folder/save)
            companion=Path(str(source)+'.byzport')
            if companion.is_file():
                shutil.copy2(companion,folder/(save+'.byzport'))
            for filename in ('BTB.SAV','STATUS.000','NEW.CAN','notebook.000'):
                path=args.data/'copy2hd/savegame'/filename
                if path.is_file():shutil.copy2(path,folder/filename)
            prefs = Path(spec['prefs_source']) if spec.get('prefs_source') else next(p for p in (args.data/'copy2hd/savegame/gamepref_000',
                         args.data/'copy2hd/savegame/GAMEPREF.000', args.data/'copy2hd/goldsave/GAMEPREF.000') if p.is_file())
            shutil.copy2(prefs,folder/'GAMEPREF.000')
            dump=subprocess.check_output([str(PORT/'byzantine-cc'),'--save-dump',str(folder/save),
                                          '--lcc',str(self.engine/'SHOW.LCC')],text=True,errors='replace')
            cd=re.search(r'^cd=([1-6])\b',dump,re.MULTILINE)
            saved_scene=re.search(r'chapter_scenes=\d+ scene=(\w+)',dump)
            if not cd or not saved_scene or saved_scene[1].lower()!=scene.lower():
                raise ValueError('seed save does not match scene: '+name)
            spec=dict(spec,scene=scene.upper(),disc=int(cd[1]),save_sha256=digest(folder/save),
                      prefs_source=str(folder/'GAMEPREF.000'),prefs_sha256=digest(folder/'GAMEPREF.000'),
                      runtime_save_sha256=digest(folder/(save+'.byzport')) if companion.is_file() else None)
            self.seeds[name] = spec
        self.manifest = None
        if args.media_manifest:
            self.manifest=output/'media_isos.txt'
            lines=[str(Path(p).resolve()) for p in args.media_manifest.read_text().splitlines() if p]
            self.manifest.write_text('\n'.join(lines)+'\n')
        self.overlays={}
        if self.manifest:
            from media_sources import disc_number
            sources={}
            for source in self.manifest.read_text().splitlines():
                path=Path(source)
                number=disc_number(path)
                if number in sources and sources[number]!=path:raise ValueError('ambiguous media disc')
                sources[number]=path
            for seed in self.seeds.values():
                disc=seed['disc']
                if disc not in sources:raise ValueError(f'missing seed disc {disc}')
                if disc not in self.overlays:
                    result=subprocess.run([sys.executable,str(PORT/'prepare_disc_anims.py'),str(self.assets),
                                           str(sources[disc]),str(disc)],capture_output=True,text=True,check=True,timeout=90)
                    self.overlays[disc]=result.stdout.strip()

    def capacity(self):
        return self.runs < self.args.max_runs and time.monotonic() < self.deadline

    def run(self, plan, suffix='', roundtrip=None, assertions=None, require_media=False,settings_expectations=None,save_keys=None):
        if not self.capacity():raise BudgetReached()
        self.runs += 1
        folder=self.output/f'trial-{self.runs:05d}{suffix}'
        folder.mkdir()
        seed=self.seeds[plan.seed]
        saves=folder/'saves';shutil.copytree(self.output/'inputs'/plan.seed,saves)
        # Runtime configuration never comes from a previous mutated trial.
        profile=validate_profile(getattr(self.args,'settings_profile',None))
        (folder/'settings.ini').write_text(profile['ini'] if profile else '[game]\nautosave=0\ngamepad=0\n')
        if profile:(saves/'GAMEPREF.000').write_bytes(struct.pack('<7I',*profile['gameprefs']))
        save_keys=save_keys or ['F5','F9']
        if len(save_keys)!=2 or any(not re.fullmatch(r'F(?:[1-9]|1[0-9]|2[0-4])',key) for key in save_keys):
            raise ValueError('invalid settings save/load bindings')
        lines=[];steps={0:0};names=['p0000']
        for value in seed.get('warmup',[]):
            for command in Action.read(value).commands:
                lines.append(command);steps[len(lines)]=0
        lines.append('snapshot p0000')
        for step, action in enumerate(plan.actions,1):
            for command in action.commands:
                lines.append(command);steps[len(lines)]=step
            name=f'p{step:04d}';names.append(name);lines.append('snapshot '+name)
        if roundtrip:
            lines+=['key '+save_keys[0],'snapshot saved']
            lines+=roundtrip.commands+['snapshot changed','key '+save_keys[1],'snapshot loaded']
            lines+=roundtrip.commands+['snapshot replayed']
        lines+=['quit']
        (folder/'actions.txt').write_text('\n'.join(lines)+'\n')
        scene=seed['scene']
        command=[str(self.args.player),str(self.tables/'cc'/scene),str(self.tables/'cb'/scene),str(seed['label']),
                 '--save',str(saves/seed['save']),'--engine',str(self.engine),'--config',str(folder/'settings.ini'),
                 '--backgrounds',str(self.assets),'--anims',str(self.assets),'--sounds',str(self.assets),
                 '--fonts',str(self.assets/'copy2hd/fonts'),'--messages',str(self.assets/'setupart/MESSAGES.TXT'),
                 '--gameprefs',str(saves/'GAMEPREF.000'),'--inventor',str(self.assets/'inventor.txt'),
                 '--hotitems',str(self.assets/'hotitems.txt'),'--ccb',str(self.tables/'ccb'),
                 '--ccf',str(self.tables/'ccf'),'--byz',str(self.tables/'byz'),
                 '--dialog',str(self.assets/'dialog'),'--dialog-cache',str(self.output/'decoded'),
                 '--automation',str(folder/'actions.txt'),'--automation-output',str(folder/'checkpoints'),
                 '--automation-explore','--no-transitions']
        if not self.args.with_media:
            command+=['--no-video']
            if not profile or not profile.get('audio',True):command+=['--no-audio']
        if profile:command+=['--automation-inspect-settings']
        if seed['disc'] in self.overlays:command+=['--anims-overlay',self.overlays[seed['disc']]]
        if seed.get('restore'):command+=['--automation-restore-save']
        if seed.get('entry'):command+=['--entry']
        if self.manifest:command+=['--media-isos',str(self.manifest)]
        if plan.choices:command+=['--conversation-choices',','.join(map(str,plan.choices))]
        for table in ('ci','cy','cf','cs','ca','co','cd','ce'):
            command+=['--'+table,str(self.tables/table/scene)]
        for table in ('lcx','lci','lcy'):
            command+=['--'+table,str(self.engine/('SHOW.'+table.upper()))]
        write_json(folder/'command.json',command)
        replay={'schema':1,'fingerprint_version':FINGERPRINT_VERSION,'with_media':bool(self.args.with_media),'data':str(self.args.data),'seed':dict(seed,save_source=str(self.output/'inputs'/plan.seed/seed['save'])),
                'actions':[asdict(a) for a in plan.actions],'choices':plan.choices,
                'expected':plan.expected,'player_sha256':digest(self.args.player),
                'engine_sha256':{p.name:digest(p) for p in self.engine.iterdir() if p.is_file()},
                'media_manifest':str(self.manifest) if self.manifest else None,
                'roundtrip':asdict(roundtrip) if roundtrip else None,
                'behavior_assertions':assertions or [],'require_media':require_media,
                'settings_profile':profile,'settings_expectations':settings_expectations or [],'save_keys':save_keys}
        write_json(folder/'replay.json',replay)
        remaining=self.deadline-time.monotonic()
        timeout=min(self.args.timeout,max(.01,remaining))
        code, expired=execute(command,folder,timeout)
        states=[];validation={'behavior':[],'media':None}
        try:
            for name in names:
                path=folder/'checkpoints'/(name+'.json')
                if path.is_file():
                    state=json.loads(path.read_text());fingerprint(state);states.append(state)
            log=(folder/'player.log').read_text(errors='replace')
            issue=classify(log,code,expired,states)
            if expired and timeout < self.args.timeout:raise BudgetReached()
            if not issue and len(states)!=len(names):
                issue=Issue('missing_checkpoint','error','expected checkpoint missing')
            if not issue:
                for i, expected in enumerate(plan.expected):
                    if i<len(states) and fingerprint(states[i])!=expected:
                        issue=Issue('replay_difference','review',f'prefix checkpoint {i} differs',states[i]['scene'])
                        break
            if not issue:issue=wait_issue(states[-1],plan.actions[-1] if plan.actions else None)
            for before,after in zip(states,states[1:]):
                validation['behavior'].extend(transition_checks(before,after))
            if not issue:
                failed=next((c for c in validation['behavior'] if not c['passed']),None)
                if failed:issue=Issue('behavior_contract','error',failed['id'],states[-1]['scene'])
            if self.args.with_media:
                validation['media']=media_evidence(log)
                if not issue and validation['media']['errors']:
                    issue=Issue('media_failure','error',validation['media']['errors'][0],states[-1]['scene'] if states else '')
                if not issue and validation['media']['unavailable']:
                    issue=Issue('media_unavailable','review',validation['media']['unavailable'][0],states[-1]['scene'] if states else '')
            validation['directed']=[check_assertion(states,a) for a in (assertions or [])] if not issue else []
            if not issue and any(not c['passed'] for c in validation['directed']):
                failed=next(c for c in validation['directed'] if not c['passed'])
                issue=Issue('behavior_contract','error','expected outcome: '+str(failed['path']),states[-1]['scene'])
            if require_media and (not issue or issue.severity=='review'):
                evidence=validation['media'] or {}
                if not (evidence.get('completed_playbacks') and evidence.get('audio_queues')):
                    issue=Issue('media_coverage_missing','error','completed clip and queued audio required',states[-1]['scene'] if states else '')
            validation['settings']=[]
            for expected in settings_expectations or []:
                if issue:break
                index=expected['at']
                if not 0<=index<len(states):raise ValueError('settings checkpoint missing')
                validation['settings'].extend(check_settings(states[index],expected))
            if profile and settings_expectations and not issue:
                validation['settings'].extend(check_flow(states,log))
            if not issue and any(not c['passed'] for c in validation['settings']):
                failed=next(c for c in validation['settings'] if not c['passed'])
                issue=Issue('settings_contract','error',failed['id'],states[-1]['scene'])
            if not issue and roundtrip:
                snapshots={name:json.loads((folder/'checkpoints'/(name+'.json')).read_text())
                           for name in ('saved','changed','loaded','replayed')}
                different=differences(persistence_projection(snapshots['saved']),persistence_projection(snapshots['loaded']),SAVE_FIELDS)
                different+=differences(persistence_projection(snapshots['changed']),persistence_projection(snapshots['replayed']),REPLAY_FIELDS)
                if different:issue=Issue('save_roundtrip_difference','review',','.join(sorted(set(different))),states[-1]['scene'])
                if profile and settings_expectations and not issue:
                    last=settings_expectations[-1]
                    for name in ('saved','loaded'):
                        validation['settings'].extend(check_settings(snapshots[name],last))
                    if self.args.with_media:
                        placements=re.findall(r'^video name=\S+ mode=(?:1|2|4) placement=(2x640x400|1x)',log,re.MULTILINE)
                        wanted='2x640x400' if last['gameprefs'][1] else '1x'
                        validation['settings'].append({'id':'native_video_placement','expected':wanted,
                            'actual':placements,'passed':bool(placements) and all(p==wanted for p in placements)})
                    saved_file=saves/'quicksave.btb'
                    validation['settings'].append({'id':'rebound_quicksave_created','passed':saved_file.is_file()})
                    validation['settings'].append({'id':'rebound_quickload_committed','passed':'quickload committed scene=' in log})
                    validation['settings'].append({'id':'camera_restored','passed':snapshots['saved']['panorama']==snapshots['loaded']['panorama']})
                    if self.capacity():
                        self.runs+=1
                        validation['settings'].extend(verify_restart(folder,last,min(self.args.timeout,max(.01,self.deadline-time.monotonic())),execute))
                    else:validation['settings'].append({'id':'fresh_process_budget','passed':False})
                    failed=next((c for c in validation['settings'] if not c['passed']),None)
                    if failed:issue=Issue('settings_budget' if failed['id']=='fresh_process_budget' else 'settings_persistence',
                        'review' if failed['id']=='fresh_process_budget' else 'error',failed['id'],states[-1]['scene'])

        except (ValueError,KeyError,OSError) as error:
            issue=Issue('invalid_checkpoint','error',str(error))
        if issue and not issue.scene:issue.scene=seed['scene'].lower()
        trial=Trial(folder,plan,states,steps,issue,code,validation)
        write_json(folder/'checks.json',validation)
        write_json(folder/'result.json',{'exit_code':code,'timed_out':expired,
                   'issue':asdict(issue) if issue else None,'checkpoints':len(states)})
        if getattr(self.args,'screenshots','all')=='findings' and issue is None:
            for image in (folder/'checkpoints').glob('*.ppm'):image.unlink()
        return trial


def pop_frontier(queue, strategy, rng):
    if strategy=='bfs':return queue.popleft()
    # Start every authentic seed before spending the budget on descendants.
    roots=[i for i,p in enumerate(queue) if not p.actions and not p.choices]
    if roots:index=roots[0]
    else:
        by_seed={}
        for i,plan in enumerate(queue):by_seed.setdefault(plan.seed,[]).append(i)
        # A branch-rich save must not starve the other starting contexts.
        selected=rng.choice(sorted(by_seed))
        indices=by_seed[selected]
        preferred=[i for i in indices if queue[i].progress_priority]
        # Reserve a uniform draw one quarter of the time. View-only routes can
        # unlock interactions, so priority never removes them from the frontier.
        if preferred and rng.random()<0.75:indices=preferred
        index=rng.choice(indices)
    queue.rotate(-index)
    plan=queue.popleft()
    queue.rotate(index)
    return plan


def select_candidates(candidates, maximum, strategy, rng):
    selected=list(candidates)
    if strategy in ('random','mixed'):rng.shuffle(selected)
    return selected[:maximum]


def explore(runner, report):
    args=runner.args
    strategy=getattr(args,'strategy','bfs')
    rng=random.Random(getattr(args,'random_seed',0))
    policy=MixedPolicy(rng) if strategy=='mixed' else None
    queue=deque()
    scheduled=set();seen={};finding_keys=set();progress_seen=set()
    report.update(progress_version=PROGRESS_VERSION, progress_states=[],
                  progress_changes=0, interaction_only_changes=0)
    report.setdefault('search', {})['priority_policy']=(policy.report()['policy'] if policy else
        'random: seed-balanced, 75% novel-progress parent / 25% uniform; bfs unchanged')
    (runner.output/'findings').mkdir()

    def enqueue(plan):
        key=plan.key()
        if key in scheduled:return
        if len(queue)>=args.max_frontier:
            report['deferred_frontier']+=1;return
        scheduled.add(key);queue.append(plan)

    def finding(trial, issue, confirmation=True, roundtrip=None):
        if issue.key() in finding_keys:return
        finding_keys.add(issue.key())
        confirmed=False;confirmation_path=None
        if confirmation and issue.kind not in ('timeout','decision_budget') and runner.capacity():
            try:
                repeat=runner.run(trial.plan,'-confirm',roundtrip=roundtrip)
                confirmation_path=str(repeat.directory)
                confirmed=bool(repeat.issue and repeat.issue.key()==issue.key())
            except BudgetReached:
                pass  # Retain the original finding even if confirmation used the remaining time.
        name=f'F{len(report["findings"])+1:04d}'
        folder=runner.output/'findings'/name;folder.mkdir()
        # Full original trial remains immutable. The bundle includes its save,
        # script, command, observations, screenshots and one replay entry point.
        for filename in ('actions.txt','command.json','player.log','result.json','replay.json','checks.json'):
            shutil.copy2(trial.directory/filename,folder/filename)
        shutil.copytree(trial.directory/'saves',folder/'saves')
        if (trial.directory/'checkpoints').is_dir():shutil.copytree(trial.directory/'checkpoints',folder/'checkpoints')
        replay=json.loads((folder/'replay.json').read_text())
        immutable=folder/'seed-input';immutable.mkdir()
        seed=runner.seeds[trial.plan.seed]
        shutil.copy2(runner.output/'inputs'/trial.plan.seed/seed['save'],immutable/seed['save'])
        companion=runner.output/'inputs'/trial.plan.seed/(seed['save']+'.byzport')
        if companion.is_file():shutil.copy2(companion,immutable/companion.name)
        shutil.copy2(seed['prefs_source'],immutable/'GAMEPREF.000')
        replay['seed']['save_source']='seed-input/'+seed['save']
        replay['seed']['prefs_source']='seed-input/GAMEPREF.000'
        if runner.manifest:
            shutil.copy2(runner.manifest,folder/'media_isos.txt')
            replay['media_manifest']='media_isos.txt'
        write_json(folder/'replay.json',replay)
        item=dict(asdict(issue),id=name,reproduced=confirmed,confirmation=confirmation_path,
                  trial=str(trial.directory),bundle=str(folder),actions=len(trial.plan.actions))
        report['findings'].append(item);write_json(folder/'finding.json',item)

    for seed in runner.seeds:enqueue(Plan(seed))

    while queue and runner.capacity() and len(seen)<args.max_states:
        plan=policy.pop(queue) if policy else pop_frontier(queue,strategy,rng)
        trial=runner.run(plan)
        report['behavior_checks']=report.get('behavior_checks',0)+len(trial.validation.get('behavior',[]))
        media=trial.validation.get('media') or {}
        report['media_playbacks']=report.get('media_playbacks',0)+len(media.get('completed_playbacks',[]))
        if trial.final:
            for decision in trial.final['exploration']['decisions']:
                observation={key:decision[key] for key in ('scene','record','group','lines','choice')}
                if observation not in report['dialogue_observations']:report['dialogue_observations'].append(observation)
        report['trials'].append({'directory':str(trial.directory),'seed':plan.seed,
                                 'depth':len(plan.actions),'lane':policy.last_lane if policy else strategy,'issue':asdict(trial.issue) if trial.issue else None})
        if trial.final:
            actual=[d['choice'] for d in trial.final['exploration']['decisions']]
            scheduled.add(Plan(plan.seed,plan.actions,actual).key())
            for alternative in decision_branches(trial,args.max_decisions):enqueue(alternative)
        if trial.issue:
            finding(trial,trial.issue)
        else:
            state=trial.final
            key=(plan.seed,fingerprint(state))
            progress=progress_fingerprint(state)
            path_progress=len({progress_fingerprint(s) for s in trial.states})
            novel_progress=progress not in progress_seen
            changed_fields=[]
            if len(trial.states)>1:
                changed_fields=differences(progress_projection(trial.states[-2]),
                                           progress_projection(state),progress_projection(state))
                if policy:policy.observe(plan,progress_fingerprint(trial.states[-2]),bool(changed_fields),
                                         trial.states[-2]['scene'],state['scene'])
                if changed_fields:report['progress_changes']+=1
                elif fingerprint(trial.states[-2])!=key[1]:report['interaction_only_changes']+=1
            if novel_progress:
                progress_seen.add(progress)
                report['progress_states'].append({'fingerprint':progress,'scene':state['scene'],
                    'seed':plan.seed,'trial':str(trial.directory),
                    'depth':len(plan.actions),'path_progress_contexts':path_progress,'replay':str(trial.directory/'replay.json')})
            report['edges'].append({'seed':plan.seed,'path':[a.id for a in plan.actions],
                                    'state':key[1],'from':plan.expected[-1] if plan.expected else None,
                                    'duplicate':key in seen,'progress':progress,'new_progress':novel_progress,
                                    'progress_changed_fields':changed_fields})
            if key in seen:
                report['duplicate_states']+=1
            else:
                seen[key]=str(trial.directory)
                report['states'].append({'seed':plan.seed,'fingerprint':key[1],'scene':state['scene'],
                                         'background':state['background'],'depth':len(plan.actions),
                                         'trial':str(trial.directory),'terminal':state['session_ended'],
                                         'progress':progress,'new_progress':novel_progress,'path_progress_contexts':path_progress})
                candidates=action_candidates(state,plan.actions[-1] if plan.actions else None)
                hashes=[fingerprint(s) for s in trial.states]
                if len(plan.actions)<args.max_depth and not state['session_ended']:
                    # Use choices actually observed on this path, rather than
                    # inventing answers for lists that have not appeared yet.
                    choices=[d['choice'] for d in state['exploration']['decisions']]
                    selected=select_candidates(candidates,args.max_actions,strategy,rng)
                    report['deferred_actions']+=max(0,len(candidates)-len(selected))
                    for action in selected:enqueue(Plan(plan.seed,plan.actions+[action],choices,hashes,novel_progress,progress,state['scene'],candidate_goal(action,state)))
                else:report['depth_frontier']+=len(candidates)
                if (args.save_every and len(seen)%args.save_every==0 and runner.capacity()
                        and not state['session_ended']):
                    action=next((a for a in candidates if a.category=='hotspot'),None)
                    if action:
                        check=runner.run(plan,'-save',roundtrip=action)
                        report['save_checks'].append({'trial':str(check.directory),'passed':check.issue is None})
                        if check.issue:finding(check,check.issue,True,action)
        if policy:report['policy_metrics']=policy.report()
        report['runs']=runner.runs
        report['frontier']=len(queue)
        write_json(runner.output/'report.json',report)
        render_report(runner.output,report)
        print(f'runs={runner.runs} states={len(seen)} progress={len(progress_seen)} frontier={len(queue)} findings={len(report["findings"])}',flush=True)
    report['frontier']=len(queue)
    if not queue:reason='frontier_exhausted_within_abstraction'
    elif runner.runs>=args.max_runs:reason='max_runs'
    elif time.monotonic()>=runner.deadline:reason='max_seconds'
    else:reason='max_states'
    report['stop_reason']=reason


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--data',type=Path)
    seed_group=parser.add_mutually_exclusive_group()
    seed_group.add_argument('--seeds',type=Path)
    seed_group.add_argument('--all-saves',action='store_true',help='start from all valid distinct numbered saves, restored without rerunning entry')
    parser.add_argument('--replay',type=Path)
    parser.add_argument('--output',type=Path)
    parser.add_argument('--strategy',choices=('bfs','random','mixed'),default='bfs')
    parser.add_argument('--random-seed',type=int,default=0,help='reproducible random search seed')
    parser.add_argument('--screenshots',choices=('all','findings'),default='all',help='retain all images or only finding images')
    parser.add_argument('--player',type=Path,default=PORT/'byzantine-play')
    parser.add_argument('--media-manifest',type=Path)
    parser.add_argument('--with-media',action='store_true',default=None,help='play actual media with dummy SDL devices; allow longer timeouts')
    parser.add_argument('--no-isos',action='store_true',help='explicit asset-only diagnostic profile')
    for flag,default in (('max-runs',80),('max-states',40),('max-depth',6),('max-frontier',512),
                         ('max-actions',24),('max-decisions',32),('save-every',8)):
        parser.add_argument('--'+flag,type=int,default=default)
    parser.add_argument('--timeout',type=float,default=30)
    parser.add_argument('--max-seconds',type=float,default=300)
    args=parser.parse_args()
    if any(getattr(args,k)<=0 for k in ('max_runs','max_states','max_frontier','max_actions','max_decisions','timeout','max_seconds')) or args.max_depth<0 or args.save_every<0:
        parser.error('budgets must be positive (depth/save-every may be zero)')
    if not 0<=args.random_seed<2**64:parser.error('random seed must be in 0..2^64-1')
    previous=None
    if args.replay and (args.all_saves or args.seeds):parser.error('replay cannot select a different seed pool')
    if args.replay:
        previous=json.loads(args.replay.read_text())
        args.settings_profile=validate_profile(previous.get('settings_profile'))
        for field in ('save_source','prefs_source'):
            if previous['seed'].get(field):
                path=Path(previous['seed'][field])
                if not path.is_absolute():previous['seed'][field]=str((args.replay.parent/path).resolve())
        if previous.get('schema')!=1:parser.error('unknown replay schema')
        if digest(previous['seed']['save_source'])!=previous['seed']['save_sha256']:
            parser.error('replay seed save hash mismatch')
        if previous['seed'].get('runtime_save_sha256'):
            companion=Path(previous['seed']['save_source']+'.byzport')
            if not companion.is_file() or digest(companion)!=previous['seed']['runtime_save_sha256']:
                parser.error('replay runtime companion hash mismatch')
        if digest(previous['seed']['prefs_source'])!=previous['seed']['prefs_sha256']:
            parser.error('replay preference hash mismatch')
        args.data=args.data or Path(previous['data'])
        if not args.media_manifest and previous['media_manifest']:
            args.media_manifest=Path(previous['media_manifest'])
            if not args.media_manifest.is_absolute():args.media_manifest=args.replay.parent/args.media_manifest
        if args.with_media is None:args.with_media=previous.get('with_media',False)
        specs=[previous['seed']]
    elif args.all_saves:specs=None
    else:specs=json.loads((args.seeds or PORT/'tests/exploration/seeds.json').read_text())
    if not args.data:parser.error('--data is required')
    args.data=args.data.resolve();args.player=args.player.resolve()
    if args.no_isos and args.media_manifest:parser.error('--no-isos conflicts with --media-manifest')
    if not previous and not args.no_isos and not args.media_manifest and (args.data/'media_isos.txt').is_file():
        args.media_manifest=args.data/'media_isos.txt'
    if previous:
        for name,expected in previous['engine_sha256'].items():
            if digest(args.data/'copy2hd/engine'/name)!=expected:parser.error('replay engine data hash mismatch: '+name)
    output=(args.output or PORT/'output'/('explore-'+time.strftime('%Y%m%dT%H%M%S')+'-'+str(os.getpid()))).resolve()
    output.mkdir(parents=True,exist_ok=False)
    report={'schema':1,'status':'RUNNING','scope':'bounded dummy-SDL exploration; no physical or original-game oracle',
            'with_media':bool(args.with_media),
            'search':{'strategy':args.strategy,'random_seed':args.random_seed,'max_runs':args.max_runs,
                      'max_states':args.max_states,'max_depth':args.max_depth,'max_frontier':args.max_frontier,
                      'max_seconds':args.max_seconds,'screenshots':args.screenshots},
            'complete_playthrough_verified':False,'player_sha256':digest(args.player),
            'states':[],'edges':[],'findings':[],'trials':[],'save_checks':[],
            'duplicate_states':0,'deferred_frontier':0,'deferred_actions':0,'depth_frontier':0,
            'dialogue_observations':[],'behavior_checks':0,'media_playbacks':0}
    runner=None
    try:
        if args.all_saves and not previous:
            from explore_seeds import collect_save_seeds
            specs,catalogue=collect_save_seeds(args.data)
            report['seed_pool']=catalogue
            write_json(output/'seed-pool.json',dict(catalogue,seeds=specs))
        runner=Runner(args,output,specs)
        report['seeds']=list(runner.seeds.values())
        write_json(output/'report.json',report)
        if previous:
            plan=Plan(previous['seed']['id'],[Action.read(a) for a in previous['actions']],
                      previous['choices'],previous.get('expected',[]) if previous.get('fingerprint_version')==FINGERPRINT_VERSION else [])
            check=runner.run(plan,'-replay',Action.read(previous['roundtrip']) if previous['roundtrip'] else None,
                             assertions=previous.get('behavior_assertions'),require_media=previous.get('require_media',False),settings_expectations=previous.get('settings_expectations'),
                             save_keys=previous.get('save_keys'))
            report['replay']={'trial':str(check.directory),'issue':asdict(check.issue) if check.issue else None,
                              'player_changed':previous['player_sha256']!=digest(args.player),
                              'prefix_validation':previous.get('fingerprint_version')==FINGERPRINT_VERSION}
            if check.issue:report['findings'].append(asdict(check.issue))
            report['stop_reason']='replay_finished'
        else:explore(runner,report)
        report['status']='FINDINGS' if report['findings'] else 'BOUNDED'
    except BudgetReached:
        report['status']='FINDINGS' if report['findings'] else 'BOUNDED';report['stop_reason']='resource_budget'
    except KeyboardInterrupt:
        report['status']='INTERRUPTED';report['stop_reason']='user_interrupt'
    except Exception as error:
        report['status']='ERROR';report['error']=str(error)
    finally:
        report['runs']=runner.runs if runner else 0
        report['discovery_trials']=len(report['trials'])
        report['requested_run_limit_reached']=report['runs']>=args.max_runs
        report['elapsed_seconds']=round(time.monotonic()-runner.started,3) if runner else 0
        report['scenes']=sorted({s['scene'] for s in report['states']})
        write_json(output/'report.json',report)
        render_report(output,report)
    print('Report:',output/'report.json',report['status'])
    print('Browse:',output/'index.html')
    return int(report['status'] in ('ERROR','INTERRUPTED') or any(i['severity']=='error' for i in report['findings']))

if __name__=='__main__':raise SystemExit(main())
