#!/usr/bin/env python3
"""Explicit checkpoint-to-goal story contracts, including saved continuations."""
import argparse
import copy
import html
import json
from pathlib import Path
import sys
import time
from types import SimpleNamespace

from explore_game import (PORT, Runner, Plan, Action, digest, write_json,
                          persistence_projection, differences, REPLAY_FIELDS)
from behavior_checks import check_assertion


def validate_pack(pack):
    if pack.get('schema') != 1 or pack.get('kind') != 'story-milestones':
        raise ValueError('unsupported story manifest')
    if pack.get('complete_playthrough_verified') is not False:
        raise ValueError('checkpoint contracts cannot certify a complete playthrough')
    names=set()
    for case in pack['cases']:
        if case['name'] in names:raise ValueError('duplicate story name')
        names.add(case['name'])
        actions=[Action.read(a) for a in case['actions']]
        if not actions or not case['checks'] or not case['milestones']:
            raise ValueError('story needs actions, assertions and milestones')
        for assertion in case['checks']:
            if not 0<=assertion['at']<=len(actions):raise ValueError('invalid story checkpoint')
        for milestone in case['milestones']:
            if not any(a['at']==milestone['at'] for a in case['checks']):
                raise ValueError('milestone has no observable assertion')
        if not any(a['at']==len(actions) for a in case['checks']):
            raise ValueError('story has no final outcome assertion')
        cut=case.get('resume_after')
        if cut is not None and (type(cut) is not int or not 0<cut<len(actions)):
            raise ValueError('saved continuation needs an intermediate checkpoint')
        if cut is not None and case['choices']:
            raise ValueError('saved dialogue continuation needs explicit choice cursor support')
    return pack


def evaluate(states, assertions):
    checks=[]
    for assertion in assertions:
        try:checks.append(check_assertion(states,assertion))
        except (ValueError,KeyError,IndexError,TypeError) as error:
            checks.append(dict(checkpoint=assertion['at'],path=assertion['path'],passed=False,error=str(error)))
    return checks


def milestones(case,checks):
    return [dict(m,passed=bool([c for c in checks if c['checkpoint']==m['at']]) and
                 all(c['passed'] for c in checks if c['checkpoint']==m['at']))
            for m in case['milestones']]


def suffix_assertions(case,cut):
    result=[]
    for assertion in case['checks']:
        if assertion['at']<cut:continue
        a=copy.deepcopy(assertion);a['at']-=cut
        if a['op']=='equal_checkpoint':
            if a['other_at']<cut:raise ValueError('cross-cut equality needs an explicit reference')
            a['other_at']-=cut
        result.append(a)
    return result


def render(output,report):
    esc=lambda value:html.escape(str(value),quote=True)
    rows=[]
    for case in report['cases']:
        link=Path(case['trial']).relative_to(output)/'replay.json'
        failed=[m['name'] for m in case['milestones'] if not m['passed']]
        rows.append(f'<tr><td>{esc(case["name"])}</td><td>{esc(case["status"])}</td>'
                    f'<td>{esc(case["goal"])}</td><td>{esc(", ".join(failed))}</td>'
                    f'<td><a href="{esc(link)}">Replay</a></td></tr>')
    (output/'index.html').write_text('<!doctype html><meta charset="utf-8"><title>Story contracts</title>'
        '<style>body{font:16px system-ui;max-width:1200px;margin:32px auto}td,th{padding:12px;text-align:left;vertical-align:top;border-bottom:1px solid #aaa}</style>'
        f'<h1>Story contracts: {esc(report["status"])}</h1><p>{report["runs"]} player starts. '
        'Authentic checkpoints to explicit goals; complete new-game playthrough and original equivalence unverified.</p>'
        '<p><a href="report.json">Full report, checks, saved continuation and coverage gaps</a></p>'
        '<table><tr><th>Case</th><th>Status</th><th>Goal</th><th>Unmet milestones</th><th>Evidence</th></tr>'
        +''.join(rows)+'</table>')


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--data',type=Path,required=True)
    parser.add_argument('--output',type=Path,required=True)
    parser.add_argument('--contracts',type=Path,default=PORT/'tests/exploration/story-contracts.json')
    parser.add_argument('--timeout',type=float,default=90)
    parser.add_argument('--max-seconds',type=float,default=900)
    args=parser.parse_args()
    if min(args.timeout,args.max_seconds)<=0:parser.error('positive budgets required')
    pack=validate_pack(json.loads(args.contracts.read_text()))
    output=args.output.resolve();output.mkdir(parents=True,exist_ok=False)
    deadline=time.monotonic()+args.max_seconds
    options=SimpleNamespace(data=args.data.resolve(),player=PORT/'byzantine-play',with_media=False,
        max_runs=len(pack['cases'])*2,max_seconds=args.max_seconds,timeout=args.timeout,
        screenshots='all',media_manifest=None)
    if (options.data/'media_isos.txt').is_file():options.media_manifest=options.data/'media_isos.txt'
    report=dict(schema=1,kind='story-milestones',status='RUNNING',scope=pack['scope'],
        complete_playthrough_verified=False,uncovered=pack['uncovered'],contracts_sha256=digest(args.contracts),
        cases=[],findings=[],runs=0,assertions_checked=0,saved_continuations=0)
    def publish():write_json(output/'report.json',report);render(output,report)
    publish();runner=None;extra_runs=0
    try:
        runner=Runner(options,output,[c['seed'] for c in pack['cases']])
        for name,expected in pack['script_sha256'].items():
            if digest(runner.tables/'cc'/name)!=expected:raise ValueError('story evidence script hash mismatch: '+name)
        for case in pack['cases']:
            print('story start:',case['name'],flush=True)
            actions=[Action.read(a) for a in case['actions']]
            trial=runner.run(Plan(case['seed']['id'],actions,case['choices']),assertions=case['checks'])
            checks=evaluate(trial.states,case['checks'])
            item=dict(name=case['name'],goal=case['goal'],trial=str(trial.directory),
                replay=str(trial.directory/'replay.json'),checks=checks,milestones=milestones(case,checks),
                evidence=case['evidence'],issue=vars(trial.issue) if trial.issue else None,continuation=None)
            failure=any(not c['passed'] for c in checks) or bool(trial.issue)
            cut=case.get('resume_after')
            if not failure and cut is not None:
                prefix_checks=[a for a in case['checks'] if a['at']<=cut]
                prefix=runner.run(Plan(case['seed']['id'],actions[:cut],case['choices']),'-save',
                    roundtrip=actions[cut],assertions=prefix_checks)
                saved=prefix.directory/'saves/quicksave.btb'
                if prefix.issue or not saved.is_file():
                    item['continuation']=dict(status='FAIL',reason='save/load prefix failed',trial=str(prefix.directory),
                        issue=vars(prefix.issue) if prefix.issue else None);failure=True
                elif time.monotonic()>=deadline:
                    raise RuntimeError('story continuation time budget exhausted')
                else:
                    seed=dict(id=case['name']+'-resumed',scene=prefix.states[-1]['scene'].upper(),
                        save='quicksave.btb',label=0,restore=True,save_source=str(saved),
                        prefs_source=str(prefix.directory/'saves/GAMEPREF.000'))
                    resume_dir=output/(case['name']+'-continuation');resume_dir.mkdir()
                    resumed_options=copy.copy(options);resumed_options.max_runs=1
                    resumed_options.max_seconds=max(.01,deadline-time.monotonic())
                    resumed=Runner(resumed_options,resume_dir,[seed])
                    suffix=resumed.run(Plan(seed['id'],actions[cut:],[]),assertions=suffix_assertions(case,cut))
                    extra_runs+=resumed.runs
                    resumed_checks=evaluate(suffix.states,suffix_assertions(case,cut))
                    changed=[]
                    if suffix.states:
                        for actual,reference in [(suffix.states[0],prefix.states[-1]),(suffix.states[-1],trial.states[-1])]:
                            changed+=differences(persistence_projection(actual),persistence_projection(reference),REPLAY_FIELDS)
                    else:changed=['missing resumed checkpoints']
                    passed=not suffix.issue and not changed and all(c['passed'] for c in resumed_checks)
                    item['continuation']=dict(status='PASS' if passed else 'FAIL',cut=cut,trial=str(suffix.directory),
                        replay=str(suffix.directory/'replay.json'),prefix=str(prefix.directory),
                        differences=sorted(set(changed)),checks=resumed_checks,issue=vars(suffix.issue) if suffix.issue else None)
                    failure=not passed
                    report['saved_continuations']+=int(passed)
            item['status']='FAIL' if failure else 'PASS'
            report['cases'].append(item)
            report['runs']=runner.runs+extra_runs
            report['assertions_checked']+=len(checks)+len((item['continuation'] or {}).get('checks',[]))
            if failure:
                first=next((c for c in checks if not c['passed']),None)
                reason=(f'checkpoint {first["checkpoint"]}: {first["path"]}' if first else 'runtime or saved continuation differs')
                report['findings'].append(dict(kind='story_outcome',severity='error',reason=case['name']+': '+reason,
                    trial=str(trial.directory),replay=str(trial.directory/'replay.json')))
            write_json(trial.directory/'story-outcomes.json',item);publish()
            print('story',case['name'],item['status'],flush=True)
        report['status']='FINDINGS' if report['findings'] else 'PASS'
    except KeyboardInterrupt:report['status']='INTERRUPTED'
    except Exception as error:report['status']='ERROR';report['error']=str(error)
    if runner:report['runs']=runner.runs+extra_runs
    report['milestones_checked']=sum(len(c['milestones']) for c in report['cases'])
    report['milestones_passed']=sum(m['passed'] for c in report['cases'] for m in c['milestones'])
    publish();print('Report:',output/'report.json',report['status'],flush=True)
    return int(report['status']!='PASS')


if __name__=='__main__':sys.exit(main())
