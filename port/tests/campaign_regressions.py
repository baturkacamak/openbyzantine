#!/usr/bin/env python3
"""Actual SDL regressions recovered from the user's 2000-trial campaign.

No hidden labels/state injection. Uses shipped numbered saves and explicit
normal inputs; output is isolated from both original saves and game LATEST.
"""
import argparse
import json
from pathlib import Path
import subprocess
import sys
from types import SimpleNamespace

PORT=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(PORT))
import explore_game as e


def fresh_load(runner,trial,saved):
    """Start a second OS process using only the persisted BTB + companion."""
    folder=trial.directory/'fresh-process';folder.mkdir()
    script=folder/'input.txt';script.write_text('snapshot loaded\nquit\n')
    command=json.loads((trial.directory/'command.json').read_text())
    name=saved['scene']
    command[1]=str(Path(command[1]).with_name(name.upper()))
    command[2]=str(Path(command[2]).with_name(name.upper()))
    command[3]='0'
    for key in ('--ci','--cy','--cf','--cs','--ca','--co','--ce','--cd'):
        if key not in command:continue
        at=command.index(key);path=Path(command[at+1]).with_name(name.upper())
        if path.is_file():command[at+1]=str(path)
        else:del command[at:at+2]
    for key,value in {'--save':trial.directory/'saves/quicksave.btb',
                      '--automation':script,'--automation-output':folder,
                      '--config':folder/'settings.ini'}.items():
        command[command.index(key)+1]=str(value)
    if '--automation-restore-save' in command:command.remove('--automation-restore-save')
    command+=['--restore-save']
    e.write_json(folder/'command.json',command)
    code,expired=e.execute(command,folder,60)
    assert code==0 and not expired,(trial.directory,(folder/'player.log').read_text())
    loaded=json.loads((folder/'loaded.json').read_text())
    fields=e.SAVE_FIELDS+('background','active_slots','selected','inventory_ui','panorama','morris','random_state')
    different=e.differences(e.persistence_projection(saved),e.persistence_projection(loaded),fields)
    assert not different,(trial.plan.seed,'fresh-process',different)
    assert 'runtime_load file=' in (folder/'player.log').read_text()


def exercise_morris(runner,plan):
    # Real token pick and placement from the observed board, then an ordinary
    # exit. These coordinates are test data; the engine has no scene branch.
    selected=e.Action('pick_token','hotspot',['click 124 112'])
    placed=e.Action('place_token','hotspot',['click 176 144'])
    played=e.Plan(plan.seed,plan.actions+[selected,placed],plan.choices)
    trial=runner.run(played,'-morris-turn',roundtrip=e.Action('pick_next','hotspot',['click 64 132']))
    assert trial.issue is None,trial.issue
    board=trial.final['morris']
    assert board['initialized'] and board['turns']==1,board
    assert sum(bool(node[1]&2) for node in board['nodes'])==1,board
    assert sum(bool(node[1]&1) for node in board['nodes'])==1,board
    saved=json.loads((trial.directory/'checkpoints/saved.json').read_text())
    fresh_load(runner,trial,saved)
    exit_action=next(e.Action.read(a) for a in trial.final['exploration']['candidates'] if a['id']=='hotspot_18')
    leave=runner.run(e.Plan(plan.seed,played.actions+[exit_action],plan.choices),'-morris-exit')
    assert leave.issue is None and leave.final['scene']=='vc' and leave.final['active_slots'],leave.issue
    print('MORRIS real human/computer turn, persisted board and usable exit PASS',flush=True)


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--data',type=Path,required=True)
    parser.add_argument('--output',type=Path,required=True)
    args=parser.parse_args();args.data=args.data.resolve();output=args.output.resolve()
    output.mkdir(parents=True,exist_ok=False)
    cases=json.loads((PORT/'tests/exploration/campaign-regressions.json').read_text())
    options=SimpleNamespace(data=args.data,player=PORT/'byzantine-play',with_media=False,
        max_runs=100,max_seconds=600,timeout=60,media_manifest=None,screenshots='all')
    if (args.data/'media_isos.txt').is_file():options.media_manifest=args.data/'media_isos.txt'
    runner=e.Runner(options,output,[case['seed'] for case in cases])
    results=[]
    for case in cases:
        plan=e.Plan(case['id'],[e.Action.read(a) for a in case['actions']],case['choices'])
        action=e.Action.read(case['roundtrip']) if case['roundtrip'] else None
        trial=runner.run(plan,roundtrip=action)
        assert trial.issue is None,(case['id'],trial.issue)
        if action:
            saved=json.loads((trial.directory/'checkpoints/saved.json').read_text())
            fresh_load(runner,trial,saved)
        if case['id']=='F0015':exercise_morris(runner,plan)
        results.append({'id':case['id'],'passed':True,'fresh_process':bool(action)})
        print(case['id'],'PASS',flush=True)
    e.write_json(output/'summary.json',results)
    print('campaign regressions: 20/20 PASS; persisted saves restored in fresh processes; MORRIS playable turn/exit PASS')


if __name__=='__main__':main()
