#!/usr/bin/env python3
"""Asset-backed explorer contract plus a clearly synthetic fault/replay fixture."""
import argparse
import json
from pathlib import Path
import subprocess
import sys

PORT=Path(__file__).resolve().parents[1]


def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--data',type=Path,required=True)
    p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();output=a.output.resolve();output.mkdir(parents=True,exist_ok=False)
    seed=output/'map.json'
    seed.write_text('[{"id":"map","scene":"MAP","label":87,"save":"27.btb"}]\n')
    base=[sys.executable,str(PORT/'explore_game.py'),'--data',str(a.data.resolve()),'--seeds',str(seed),
          '--max-runs','6','--max-states','4','--max-depth','2','--save-every','0']
    with (output/'discovery.log').open('w') as log:
        result=subprocess.run(base+['--output',str(output/'discovery')],stdout=log,stderr=subprocess.STDOUT,timeout=90)
    r=json.loads((output/'discovery/report.json').read_text())
    assert result.returncode==0 and len(r['states'])>=2,r
    first=json.loads((output/'discovery/trial-00001/checkpoints/p0000.json').read_text())
    hotspots=[c for c in first['exploration']['candidates'] if c['category']=='hotspot']
    assert len(hotspots)==6,hotspots
    assert len(r['scenes'])>=2 and r['complete_playthrough_verified'] is False,r
    assert 1<=len(r['progress_states'])<=len(r['states']),r
    assert all('progress' in item for item in r['states']),r
    assert (output/'discovery/index.html').is_file()
    with (output/'random.log').open('w') as log:
        result=subprocess.run(base+['--strategy','random','--random-seed','314159',
            '--screenshots','findings','--output',str(output/'random')],
            stdout=log,stderr=subprocess.STDOUT,timeout=90)
    random_report=json.loads((output/'random/report.json').read_text())
    assert result.returncode==0,random_report
    assert random_report['search']['strategy']=='random' and random_report['search']['random_seed']==314159
    assert random_report['discovery_trials']<=random_report['runs']<=6
    assert not list((output/'random').glob('trial-*/checkpoints/*.ppm'))
    from importlib import import_module
    sys.path.insert(0,str(PORT))
    catalogue_module=import_module('explore_seeds')
    pool,catalogue=catalogue_module.collect_save_seeds(a.data.resolve())
    assert len(catalogue['scenes'])>3 and len(pool)>3,catalogue
    selected=[s for s in pool if s['save'] in ('26.btb','60.btb','72.btb')]
    assert len(selected)==3,selected
    restore_seeds=output/'restore-seeds.json';restore_seeds.write_text(json.dumps(selected))
    with (output/'restored.log').open('w') as log:
        result=subprocess.run([sys.executable,str(PORT/'explore_game.py'),'--data',str(a.data.resolve()),
            '--seeds',str(restore_seeds),'--max-runs','3','--max-states','3','--max-depth','0',
            '--save-every','0','--output',str(output/'restored')],stdout=log,stderr=subprocess.STDOUT,timeout=90)
    restored=json.loads((output/'restored/report.json').read_text())
    assert result.returncode==0 and len(restored['states'])==3,restored
    for trial in sorted((output/'restored').glob('trial-*')):
        command=json.loads((trial/'command.json').read_text())
        assert '--automation-restore-save' in command
        assert 'entry_skipped=1' in (trial/'player.log').read_text()
        save=Path(command[command.index('--save')+1])
        import shutil
        shutil.copy2(save,save.parent/'quicksave.btb')
        script=trial/'restore-check.txt';script.write_text('snapshot initial\nkey F9\nsnapshot loaded\nquit\n')
        command[command.index('--automation')+1]=str(script)
        command[command.index('--automation-output')+1]=str(trial/'restore-check')
        import os
        with (trial/'restore-check.log').open('w') as log:
            result=subprocess.run(command,stdout=log,stderr=subprocess.STDOUT,
                env=dict(os.environ,SDL_VIDEODRIVER='dummy',SDL_AUDIODRIVER='dummy'),timeout=60)
        assert result.returncode==0,(trial/'restore-check.log').read_text()
        initial=json.loads((trial/'restore-check/initial.json').read_text())
        loaded=json.loads((trial/'restore-check/loaded.json').read_text())
        for key in ('scene','background','layer','active_slots','effective_pool','inventory','channels','panorama','media_disc'):
            assert initial[key]==loaded[key],(trial,key)
    # This executable is only a harness failure fixture, not a game defect.
    fake=output/'synthetic-player.py'
    fake.write_text('#!/usr/bin/env python3\nprint("error: synthetic explorer fault fixture",flush=True)\nraise SystemExit(3)\n')
    fake.chmod(0o755)
    with (output/'synthetic.log').open('w') as log:
        result=subprocess.run(base+['--no-isos','--player',str(fake),'--output',str(output/'synthetic')],
                              stdout=log,stderr=subprocess.STDOUT,timeout=60)
    assert result.returncode==1
    finding=output/'synthetic/findings/F0001'
    f=json.loads((finding/'finding.json').read_text())
    assert f['kind']=='player_failure' and f['reproduced'],f
    assert (finding/'seed-input/27.btb').is_file() and (finding/'seed-input/GAMEPREF.000').is_file()
    for name,player,expected in [('fault-replayed',fake,1),('real-player-replay',PORT/'byzantine-play',0)]:
        with (output/(name+'.log')).open('w') as log:
            result=subprocess.run([sys.executable,str(PORT/'explore_game.py'),'--replay',str(finding/'replay.json'),
                                   '--player',str(player),'--output',str(output/name)],
                                  stdout=log,stderr=subprocess.STDOUT,timeout=60)
        assert result.returncode==expected,(output/(name+'.log')).read_text()
        replay=json.loads((output/name/'report.json').read_text())['replay']
        assert replay['player_changed']==(expected==0),replay
    with (output/'dialogue.log').open('w') as log:
        result=subprocess.run([sys.executable,str(PORT/'explore_game.py'),'--data',str(a.data.resolve()),
            '--seeds',str(PORT/'tests/exploration/dialogue-seed.json'),'--max-runs','4','--max-states','4',
            '--max-depth','0','--save-every','0','--output',str(output/'dialogue')],
            stdout=log,stderr=subprocess.STDOUT,timeout=90)
    dialogue=json.loads((output/'dialogue/report.json').read_text())
    assert result.returncode==0,dialogue
    observations=[d for d in dialogue['dialogue_observations'] if d['scene']=='am' and d['record']==0 and d['group']==1]
    assert {d['choice'] for d in observations}=={0,1,2},observations
    assert dialogue['runs']==3,dialogue['runs']
    print('explorer assets: MAP candidates/discovery, immutable fault bundle, replay, changed-player, seeded random retention real answer branches and saved-start/F9 equivalence PASS')

if __name__=='__main__':main()
