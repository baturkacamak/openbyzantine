#!/usr/bin/env python3
"""Seeded adversarial settings/UI combinations with real SDL input and oracles."""
import argparse
import copy
import json
from pathlib import Path
import random
import re
import subprocess
import sys
import time
from types import SimpleNamespace

from explore_game import PORT,Runner,Plan,Action,write_json
from settings_checks import check_settings

GROUPS=[['window_mode','scale_mode','vsync','cursor_scale'],
        ['master_volume','speech_volume','ambient_volume','effects_volume'],
        ['text_language','voice_language','subtitles'],
        ['autosave','skip_seen','hotspots','gamepad'],
        ['key_settings','key_backlog','key_skip','key_quicksave','key_quickload','key_autoload','key_screenshot','key_hotspots']]
OPERATIONS=[name for group in GROUPS for name in group]+['native_'+str(i) for i in range(7)]+['resize','focus']
KEYS=GROUPS[4]
BOOLS=['vsync','subtitles','hotspots','hotspots_all','autosave','skip_seen','gamepad','mute_on_focus_loss']


def initial(rng,index,media=False,languages=None):
    host={key:bool(index) if index<2 else bool(rng.getrandbits(1)) for key in BOOLS}
    host.update(window_mode=['windowed','borderless','maximized'][index%3],scale_mode=rng.choice(['fit','integer']),
                cursor_scale=[1,4][index%2],text_language='auto',voice_language='auto')
    for key in GROUPS[1]:host[key]=float(index) if index<2 else rng.choice([0.,.1,.5,.9,1.])
    bindings=rng.sample(['F'+str(i) for i in range(13,25)],len(KEYS))
    host.update(zip(KEYS,bindings))
    if languages and index>0:
        for name in ('text_language','voice_language'):host[name]=rng.choice(languages[name])
    prefs=[int(bool(index)) if index<2 else rng.randrange(2) for _ in range(7)]
    if media:prefs[5]=1
    return host,prefs


def ini(host,rng):
    w,h=rng.choice([(640,480),(800,600),(1024,768),(1280,960),(961,721)])
    sections={'display':{'mode':host['window_mode'],'scale':host['scale_mode'],'window_width':w,'window_height':h,
                         'vsync':host['vsync'],'cursor_scale':host['cursor_scale'],'mute_on_focus_loss':host['mute_on_focus_loss']},
              'audio':{key.replace('_volume',''):host[key] for key in GROUPS[1]},
              'game':{key:host[key] for key in ('autosave','skip_seen','hotspots','hotspots_all','gamepad')},
              'keys':{key[4:]:host[key] for key in KEYS},
              'language':{'text':host['text_language'],'voice':host['voice_language']}}
    sections['audio']['subtitles']=host['subtitles']
    return ''.join('['+section+']\n'+''.join(key+'='+str(int(value) if type(value) is bool else value)+'\n' for key,value in values.items()) for section,values in sections.items())


def operation(name,host,prefs,rng,serial,languages=('auto',)):
    commands=[]
    if name=='resize':
        w,h=rng.choice([(320,240),(641,481),(997,541),(1280,720),(700,1000),(1920,1080)])
        commands=['resize 641 481','resize 997 541',f'resize {w} {h}','wait 300']
    elif name=='focus':commands=['focus lost','wait 20']
    elif name.startswith('native_'):
        index=int(name[7:]);y=[133,176,221,265,310,360,400][index]
        commands=['hover 550 10','click 550 10','wait 200',f'click 480 {y}','key Escape']
        prefs[index]=1-prefs[index]
    else:
        group=next(i for i,items in enumerate(GROUPS) if name in items);row=GROUPS[group].index(name)
        commands=['key '+host['key_settings']]+['key Down']*(group+1)+['key Return']+['key Down']*(row+1)
        direction=rng.choice([-1,1]);commands+=['key '+('Right' if direction>0 else 'Left')]
        if name=='window_mode':host[name]=['windowed','borderless','maximized'][(['windowed','borderless','maximized'].index(host[name])+direction)%3]
        elif name=='scale_mode':
            if host['window_mode']!='windowed':host[name]='integer' if host[name]=='fit' else 'fit'
        elif name=='cursor_scale':host[name]=host[name]%4+1
        elif name in GROUPS[1]:host[name]=round(max(0,min(1,host[name]+direction*.1)),6)
        elif name in ('text_language','voice_language'):
            codes=languages[name] if isinstance(languages,dict) else languages
            at=codes.index(host[name]);host[name]=codes[(at+direction)%len(codes)]
        elif name=='hotspots':
            host['hotspots'],host['hotspots_all']=(True,False) if not host['hotspots'] else ((True,True) if not host['hotspots_all'] else (False,True))
        elif name in KEYS:
            free=[f'F{i}' for i in range(13,25) if f'F{i}' not in [host[key] for key in KEYS]]
            host[name]=rng.choice(free);commands+=['key '+host[name]]
        else:host[name]=not host[name]
        commands+=['key '+host['key_settings'],'wait 40']
    return Action.read({'id':'settings_'+str(serial),'category':'settings','commands':commands})


def expectation(at,host,prefs,muted=False):
    return {'at':at,'host':copy.deepcopy(host),'gameprefs':list(prefs),'muted':muted,'require_audio':True}


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--data',type=Path,required=True);parser.add_argument('--output',type=Path,required=True)
    parser.add_argument('--player',type=Path,default=PORT/'byzantine-play')
    parser.add_argument('--cases',type=int,default=32);parser.add_argument('--operations',type=int,default=5)
    parser.add_argument('--random-seed',type=int,default=0);parser.add_argument('--with-media',action='store_true')
    parser.add_argument('--max-seconds',type=float,default=900);parser.add_argument('--timeout',type=float,default=120)
    args=parser.parse_args()
    if not 1<=args.cases<=10000 or not 1<=args.operations<=30 or min(args.timeout,args.max_seconds)<=0:parser.error('invalid fuzz budgets')
    rng=random.Random(args.random_seed);output=args.output.resolve();output.mkdir(parents=True,exist_ok=False)
    routes=json.loads((PORT/'tests/exploration/campaign-regressions.json').read_text())
    if args.with_media:routes=[case for case in routes if case['id']=='F0005']
    options=SimpleNamespace(data=args.data.resolve(),player=args.player.resolve(),with_media=args.with_media,
        max_runs=args.cases*2,max_seconds=args.max_seconds,timeout=args.timeout,screenshots='all',media_manifest=None,settings_profile=None)
    if (options.data/'media_isos.txt').is_file():options.media_manifest=options.data/'media_isos.txt'
    report={'schema':1,'kind':'settings-fuzz','status':'RUNNING','random_seed':args.random_seed,'with_media':args.with_media,
            'runs':0,'cases':[],'findings':[],'settings_checked':0,'scope':'real SDL UI events/dummy devices; private INI/preferences/saves; no physical monitor/controller/speaker validation'}
    start=time.monotonic();runner=None
    try:
        runner=Runner(options,output,[case['seed'] for case in routes])
        registry=subprocess.check_output([str(options.player),'--list-languages','--backgrounds',str(runner.assets),
            '--config',str(output/'language-probe.ini')],text=True)
        languages={'text_language':['auto'],'voice_language':['auto']}
        for code,text,voice in re.findall(r'^language_pack code=(\S+).* text=(0|1) voice=(0|1)',registry,re.MULTILINE):
            if text=='1':languages['text_language'].append(code)
            if voice=='1':languages['voice_language'].append(code)
        report['available_languages']=languages
        for index in range(args.cases):
            if not runner.capacity():break
            route=routes[index%len(routes)];host,prefs=initial(rng,index,args.with_media,languages)
            baseline={'schema':1,'ini':ini(host,rng),'gameprefs':list(prefs),'audio':True};options.settings_profile=baseline
            actions=[];expect=[expectation(0,host,prefs)];ops=[OPERATIONS[index%len(OPERATIONS)]]
            ops+=rng.choices(OPERATIONS,k=args.operations-1)
            if args.with_media:ops=['native_1']+[op for op in ops if op not in ('native_5','native_1')]
            game=list(route['actions']);covered=[]
            for name in ops:
                actions.append(operation(name,host,prefs,rng,len(actions),languages))
                expect.append(expectation(len(actions),host,prefs,host['mute_on_focus_loss'] if name=='focus' else False));covered.append(name)
                if name=='focus':
                    actions.append(Action('focus_restore','settings',['focus gained','wait 20']));expect.append(expectation(len(actions),host,prefs))
                if game and rng.random()<.5:
                    actions.append(Action.read(game.pop(0)));expect.append(expectation(len(actions),host,prefs))
            for command in game:
                actions.append(Action.read(command));expect.append(expectation(len(actions),host,prefs))
            print('settings case',index+1,'seed',route['seed']['id'],'operations',','.join(covered),flush=True)
            trial=runner.run(Plan(route['seed']['id'],actions,route['choices']),roundtrip=Action('post_load_camera','camera',['key Left']),
                settings_expectations=expect,require_media=args.with_media,save_keys=[host['key_quicksave'],host['key_quickload']])
            checks=list(trial.validation.get('settings',[]));error=vars(trial.issue) if trial.issue else None
            item={'index':index,'trial':str(trial.directory),'replay':str(trial.directory/'replay.json'),'operations':covered,
                  'status':('REVIEW' if error['severity']=='review' else 'FAIL') if error else 'PASS','issue':error,'checks':checks,'initial':baseline,'media':trial.validation.get('media') or {}}
            report['cases'].append(item);report['runs']=runner.runs
            report['settings_checked']+=len(checks)
            if error:report['findings'].append(dict(error,trial=str(trial.directory)))
            write_json(trial.directory/'settings-results.json',item);write_json(output/'report.json',report)
            if not error:
                for image in trial.directory.glob('checkpoints/*.ppm'):image.unlink()
                for image in trial.directory.glob('fresh-settings/*.ppm'):image.unlink()
            print('settings result',index+1,item['status'],error or '',flush=True)
        report['status']='FINDINGS' if report['findings'] else ('PASS' if len(report['cases'])==args.cases else 'BOUNDED')
    except KeyboardInterrupt:report['status']='INTERRUPTED'
    except Exception as error:report['status']='ERROR';report['error']=str(error)
    report['elapsed_seconds']=round(time.monotonic()-start,3)
    report['unique_initial_configurations']=len({json.dumps(case['initial'],sort_keys=True) for case in report['cases']})
    report['limitations']=['dummy SDL; physical monitor, speakers and controller input are not validated']
    if report.get('available_languages')=={'text_language':['auto'],'voice_language':['auto']}:
        report['limitations'].append('only base-language auto is installed; no fabricated language pack')
    report['media_playbacks']=sum(len(case['media'].get('completed_playbacks',[])) for case in report['cases'])
    report['operation_coverage']=sorted({operation for case in report['cases'] for operation in case['operations'] if case['status']=='PASS'})
    write_json(output/'report.json',report)
    print('Report:',output/'report.json',report['status'],flush=True)
    return int(report['status'] in ('ERROR','INTERRUPTED') or any(f['severity']=='error' for f in report['findings']))


if __name__=='__main__':sys.exit(main())
