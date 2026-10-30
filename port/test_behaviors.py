#!/usr/bin/env python3
"""Directed observable outcomes plus optional real-media completion checks."""
import argparse
import json
from pathlib import Path
from types import SimpleNamespace
import sys

from explore_game import PORT,Runner,Plan,Action,write_json,digest
from behavior_checks import check_assertion


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--data',type=Path,required=True)
    parser.add_argument('--output',type=Path,required=True)
    parser.add_argument('--contracts',type=Path,default=PORT/'tests/exploration/behavior-contracts.json')
    parser.add_argument('--with-media',action='store_true')
    parser.add_argument('--timeout',type=float,default=240)
    parser.add_argument('--max-seconds',type=float,default=900)
    args=parser.parse_args()
    pack=json.loads(args.contracts.read_text())
    if pack['schema']!=1:parser.error('unsupported behavior contract schema')
    cases=[case for case in pack['cases'] if not args.with_media or case['media']]
    output=args.output.resolve();output.mkdir(parents=True,exist_ok=False)
    options=SimpleNamespace(data=args.data.resolve(),player=PORT/'byzantine-play',with_media=args.with_media,
        max_runs=len(cases),max_seconds=args.max_seconds,timeout=args.timeout,screenshots='all',media_manifest=None)
    if (options.data/'media_isos.txt').is_file():options.media_manifest=options.data/'media_isos.txt'
    report={'schema':1,'kind':'behavior-contracts','status':'RUNNING','with_media':args.with_media,
            'scope':pack['scope'],'contracts_sha256':digest(args.contracts),'cases':[],'findings':[],'runs':0}
    write_json(output/'report.json',report)
    runner=None
    try:
        runner=Runner(options,output,[case['seed'] for case in cases])
        for case in cases:
            print('behavior start:',case['name'],'media='+str(args.with_media),flush=True)
            trial=runner.run(Plan(case['seed']['id'],[Action.read(a) for a in case['actions']],case['choices']),
                             assertions=case['checks'],require_media=args.with_media)
            checks=[];failure=None
            try:
                checks=[check_assertion(trial.states,assertion) for assertion in case['checks']]
                if any(not check['passed'] for check in checks):failure='expected observable outcome differs'
            except (ValueError,KeyError,IndexError,TypeError) as error:failure=str(error)
            evidence=trial.validation.get('media') or {}
            if args.with_media and not (evidence.get('completed_playbacks') and evidence.get('audio_queues')):
                failure='actual completed clip and queued audio evidence required'
            status='FAIL' if failure or (trial.issue and trial.issue.severity=='error') else ('REVIEW' if trial.issue else 'PASS')
            item={'name':case['name'],'status':status,'checks':checks,'failure':failure,
                  'trial':str(trial.directory),'replay':str(trial.directory/'replay.json'),
                  'issue':vars(trial.issue) if trial.issue else None,'media':evidence,'evidence':case['evidence']}
            report['cases'].append(item);report['runs']=runner.runs
            if status!='PASS':report['findings'].append({'kind':'behavior_outcome','severity':'error' if status=='FAIL' else 'review',
                'reason':case['name']+': '+(failure or trial.issue.reason),'trial':str(trial.directory)})
            write_json(trial.directory/'behavior-outcomes.json',item)
            write_json(output/'report.json',report)
            print('behavior',case['name'],status,flush=True)
        report['status']='FINDINGS' if report['findings'] else 'PASS'
    except KeyboardInterrupt:
        report['status']='INTERRUPTED'
    except Exception as error:
        report['status']='ERROR';report['error']=str(error)
    if runner:report['runs']=runner.runs
    report['assertions_checked']=sum(len(c['checks']) for c in report['cases'])
    write_json(output/'report.json',report)
    print('Report:',output/'report.json',report['status'])
    return int(report['status'] in ('ERROR','INTERRUPTED') or any(f['severity']=='error' for f in report['findings']))


if __name__=='__main__':sys.exit(main())
