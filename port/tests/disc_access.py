#!/usr/bin/env python3
"""Synthetic VM host handoff with authentic ISOs; never claimed a game playthrough."""
import argparse
import json
import os
from pathlib import Path
import struct
import subprocess

PORT=Path(__file__).resolve().parents[1]
def instruction(op,*values):
    return bytes([op])+b''.join(bytes([kind])+struct.pack('<H',value) for kind,value in values)
def run(output, manifest, disc, expected, failure=False):
    output.mkdir()
    # Set local 0 once, increment before and after the pause. A replay from
    # the probe result would produce 3, while a lost prefix would produce 1.
    code=[instruction(0xe1,(2,10)),instruction(0x67,(64,0),(2,0)),
          instruction(0x74,(64,1),(2,0)),instruction(0x9b,(2,20)),
          instruction(0x74,(64,1),(2,0)),instruction(0x6f),
          instruction(0xe1,(2,20)),instruction(0x8d,(64,disc)),instruction(0x6f)]
    (output/'TEST').write_bytes(struct.pack('<H',len(code))+b''.join(code))
    (output/'CB').write_bytes(b'')
    (output/'actions.txt').write_text('snapshot after\nquit\n')
    command=[str(PORT/'byzantine-play'),str(output/'TEST'),str(output/'CB'),'10',
             '--config',str(output/'config.ini'),'--no-audio','--no-video','--no-transitions',
             '--backgrounds',str(output),'--anims',str(output),'--media-isos',str(manifest),
             '--automation',str(output/'actions.txt'),'--automation-output',str(output/'shots')]
    with (output/'player.log').open('w') as log:
        result=subprocess.run(command,stdout=log,stderr=subprocess.STDOUT,
                              env=dict(os.environ,SDL_VIDEODRIVER='dummy',SDL_AUDIODRIVER='dummy'),timeout=90)
    text=(output/'player.log').read_text()
    if failure:
        assert result.returncode!=0 and 'ISO is missing or invalid' in text,text
        assert not (output/'shots/after.json').exists()
    else:
        assert result.returncode==0,text
        state=json.loads((output/'shots/after.json').read_text())
        assert state['variables']['0']==2,state['variables']
        assert state['media_disc']==expected,state
        assert f'selected={expected}' in text and 'prompt=0' in text,text
        assert 'automation_complete' in text,text

def media_collision(output):
    import zipfile
    output.mkdir()
    isos=[]
    for disc,color in ((3,'red'),(4,'blue')):
        avi=output/(color+'.avi')
        subprocess.run(['ffmpeg','-v','error','-f','lavfi','-i',f'color=c={color}:s=32x24:r=1',
                        '-frames:v','1','-c:v','mjpeg','-threads','1',str(avi)],check=True)
        iso=output/(color+'.iso')
        # Synthetic archive fixtures, not original game/volume evidence.
        with zipfile.ZipFile(iso,'w') as z:
            z.writestr('CD.BYZ',str(disc)+'\r\n')
            z.writestr('anims/zz/unused.rle','not loaded by this test')
            z.write(avi,'avi/same.avi' if disc==4 else 'video/slide/same.avi')
        isos.append(iso)
    manifest=output/'sources.txt';manifest.write_text(''.join(str(p)+'\n' for p in isos))
    code=[instruction(0xe1,(2,10)),instruction(0x8d,(64,3)),
          b'\xbd\x02\x00\x00\x40\x00\x00\x00\x00',
          instruction(0x8d,(64,4)),b'\xbd\x02\x00\x00\x40\x00\x00\x00\x00',instruction(0x6f)]
    (output/'TEST').write_bytes(struct.pack('<H',len(code))+b''.join(code))
    (output/'CB').write_bytes(b'');(output/'CF').write_bytes(b'same\0source\0\0')
    (output/'actions.txt').write_text('snapshot after\nquit\n')
    # A stale flat file with the same name must not mask the selected ISO.
    (output/'video/slide').mkdir(parents=True)
    (output/'video/slide/same.avi').write_bytes((output/'blue.avi').read_bytes())
    command=[str(PORT/'byzantine-play'),str(output/'TEST'),str(output/'CB'),'10',
             '--config',str(output/'config.ini'),'--no-audio','--no-transitions',
             '--backgrounds',str(output),'--anims',str(output),'--media-isos',str(manifest),
             '--cf',str(output/'CF'),'--dialog-cache',str(output/'decoded'),
             '--automation',str(output/'actions.txt'),'--automation-output',str(output/'shots')]
    with (output/'player.log').open('w') as log:
        result=subprocess.run(command,stdout=log,stderr=subprocess.STDOUT,
                              env=dict(os.environ,SDL_VIDEODRIVER='dummy',SDL_AUDIODRIVER='dummy'),timeout=60)
    text=(output/'player.log').read_text();assert result.returncode==0,text
    assert text.count('iso_media extracted=')==2 and 'reason=decode_failed' not in text,text
    frames=list((output/'decoded').rglob('f0001.jpg'));assert len(frames)==2,frames
    colors=set()
    for frame in frames:
        raw=subprocess.check_output(['ffmpeg','-v','error','-i',str(frame),'-f','rawvideo','-pix_fmt','rgb24','-'])
        red=sum(raw[0::3])/len(raw[0::3]);blue=sum(raw[2::3])/len(raw[2::3])
        colors.add('red' if red>200 and blue<30 else 'blue' if blue>200 and red<30 else 'wrong')
    assert colors=={'red','blue'},colors

def real_map_cases(output, data, manifest):
    import shutil, sys
    sys.path.insert(0,str(PORT))
    from gameplay_tests import prepare
    output.mkdir();engine,tables=prepare(data,output)
    for disc,label in ((1,134),(2,137),(3,140),(4,143),(5,147),(6,150)):
        folder=output/('cd'+str(disc));folder.mkdir()
        save=folder/'27.btb';shutil.copy2(data/'copy2hd/savegame/27.btb',save)
        shutil.copy2(data/'copy2hd/goldsave/GAMEPREF.000',folder/'GAMEPREF.000')
        actions='snapshot selected\n'+('key F5\nkey F9\nsnapshot loaded\n' if disc==3 else '')+'quit\n'
        (folder/'actions.txt').write_text(actions)
        command=[str(PORT/'byzantine-play'),str(tables/'cc/MAP'),str(tables/'cb/MAP'),str(label),
                 '--save',str(save),'--config',str(folder/'config.ini'),'--engine',str(engine),
                 '--backgrounds',str(data),'--anims',str(data),'--gameprefs',str(folder/'GAMEPREF.000'),
                 '--media-isos',str(manifest),'--no-audio','--no-video','--no-transitions',
                 '--automation',str(folder/'actions.txt'),'--automation-output',str(folder/'shots')]
        for table in ('ci','cy','cf','cs','ca','co','cd','ce'):
            command+=['--'+table,str(tables/table/'MAP')]
        for table in ('lcx','lci','lcy'):
            command+=['--'+table,str(engine/('SHOW.'+table.upper()))]
        with (folder/'player.log').open('w') as log:
            result=subprocess.run(command,stdout=log,stderr=subprocess.STDOUT,
                                  env=dict(os.environ,SDL_VIDEODRIVER='dummy',SDL_AUDIODRIVER='dummy'),timeout=90)
        text=(folder/'player.log').read_text();assert result.returncode==0,text
        state=json.loads((folder/'shots/selected.json').read_text())
        assert state['scene']=='map' and state['media_disc']==disc and state['active_slots'],state
        assert f'media_disc requested={disc} selected={disc}' in text and 'automation_complete' in text,text
        if disc==3:
            loaded=json.loads((folder/'shots/loaded.json').read_text())
            assert loaded['media_disc']==3 and loaded['variable_pool']==state['variable_pool'],loaded
            dump=subprocess.check_output([str(PORT/'byzantine-cc'),'--save-dump',str(folder/'quicksave.btb'),
                                          '--lcc',str(engine/'SHOW.LCC')],text=True)
            assert 'cd=3 ' in dump,dump

def source_configuration(output, paths):
    import hashlib
    output.mkdir();root=output/'ISO directory';root.mkdir()
    for number,path in enumerate(paths): (root/f'volume {number}.ISO').symlink_to(path)
    config=output/'remembered.json';manifest=output/'manifest.txt'
    command=['python3',str(PORT/'media_sources.py'),'--config',str(config),'--output',str(manifest)]
    subprocess.run(command+['--root',str(root)],check=True,stdout=subprocess.DEVNULL)
    before=manifest.read_bytes();saved=config.read_bytes()
    subprocess.run(command,check=True,stdout=subprocess.DEVNULL)
    assert manifest.read_bytes()==before and config.read_bytes()==saved
    (root/'volume 3.ISO').unlink()
    result=subprocess.run(command,capture_output=True,text=True)
    assert result.returncode!=0 and 'Missing Byzantine ISO' in result.stderr,result.stderr
    assert manifest.read_bytes()==before and config.read_bytes()==saved

if __name__=='__main__':
    p=argparse.ArgumentParser();p.add_argument('--data',type=Path);p.add_argument('--manifest',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();a.output=a.output.resolve();a.output.mkdir(exist_ok=False)
    paths=a.manifest.resolve().read_text().splitlines()
    # Alias every ISO so neither original filename nor containing folder can
    # identify its disc; runtime must read the original volume marker.
    aliases=a.output/'aliases';aliases.mkdir()
    names=[]
    for n,path in enumerate(paths):
        target=aliases/f'volume with spaces {n}.ISO';target.symlink_to(path);names.append(str(target))
    all_discs=a.output/'all.txt';all_discs.write_text('\n'.join(names)+'\n')
    run(a.output/'disc3',all_discs,3,3)
    run(a.output/'disc7',all_discs,7,1)
    fallback=a.output/'fallback.txt';fallback.write_text('\n'.join(names[1:])+'\n')
    run(a.output/'fallback2',fallback,7,2)
    absent=a.output/'empty.txt';absent.write_text('')
    run(a.output/'missing',absent,3,None,True)
    source_configuration(a.output/'configuration',paths)
    media_collision(a.output/'media-collision')
    if a.data:real_map_cases(a.output/'real-map',a.data.resolve(),all_discs)
    (a.output/'report.json').write_text(json.dumps({'status':'PASS','synthetic_host_cases':4,
        'media_collision':'PASS (generated AVI/archive fixtures)', 'configuration':'PASS',
        'shipped_map_discs':list(range(1,7)) if a.data else [],
        'save_disc_roundtrip':'PASS' if a.data else 'NOT_RUN',
        'scope':'dummy SDL; no physical display/audio or full playthrough claim'},indent=2)+'\n')
    print('disc access: renamed ISOs, state continuation, disc7 fallback and missing disc PASS')
