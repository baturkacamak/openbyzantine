#!/usr/bin/env python3
"""Unmodified original EXE startup in a private Wine prefix and nested X server.

A successful launch is not a gameplay oracle. Captures remain UNVERIFIED until
same-save/action reference checkpoints are independently recorded and compared.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess
import time
import tempfile
import struct
import zlib


def xwd_png(source, destination):
    """Decode TrueColor/DirectColor XWD with its actual pitch and pixel size."""
    raw=source.read_bytes()
    h=struct.unpack('>25I',raw[:100])
    width,height,bits,pitch=h[4],h[5],h[11],h[12]
    if h[1]!=7 or h[13] not in (4,5) or bits not in (16,24,32):
        raise ValueError('unsupported XWD capture layout')
    pixels=raw[h[0]+12*h[19]:];stride=bits//8;rows=[]
    for y in range(height):
        row=bytearray([0])
        for x in range(width):
            at=y*pitch+x*stride
            pixel=int.from_bytes(pixels[at:at+stride],'little' if h[7]==0 else 'big')
            for mask in h[14:17]:
                shift=(mask & -mask).bit_length()-1
                row.append(((pixel & mask)>>shift)*255//(mask>>shift))
        rows.append(row)
    def chunk(kind, data):
        return struct.pack('>I',len(data))+kind+data+struct.pack('>I',zlib.crc32(kind+data)&0xffffffff)
    destination.write_bytes(b'\x89PNG\r\n\x1a\n'+chunk(b'IHDR',struct.pack('>IIBBBBB',width,height,8,2,0,0,0))+
                            chunk(b'IDAT',zlib.compress(b''.join(rows)))+chunk(b'IEND',b''))


def prepare_volume(iso, parent):
    stat=iso.stat()
    identity={'iso':str(iso.resolve()),'size':stat.st_size,'mtime_ns':stat.st_mtime_ns,'version':1}
    key=hashlib.sha256(json.dumps(identity,sort_keys=True).encode()).hexdigest()[:16]
    target=parent/key
    if (target/'ready.json').is_file():return target
    parent.mkdir(parents=True,exist_ok=True)
    temporary=Path(tempfile.mkdtemp(prefix='.volume-',dir=parent))
    try:
        subprocess.run(['7z','x','-y',str(iso),'CD.BYZ','copy2hd/*','anims/*','backs/*',
                        'nodes/*','inventor/*','sounds/*','dialog/*','video/*','avi/*','-o'+str(temporary)],
                       check=True,stdout=subprocess.DEVNULL,stderr=subprocess.DEVNULL,timeout=120)
        (temporary/'ready.json').write_text(json.dumps(identity,indent=2)+'\n')
        try:
            temporary.rename(target)
        except OSError:
            if not (target/'ready.json').is_file():raise
    finally:
        if temporary.exists():shutil.rmtree(temporary)
    return target


def probe(data, exe, iso, output):
    output = Path(output).resolve()
    output.mkdir()
    result = {'status':'UNVERIFIED', 'exe_sha256':hashlib.sha256(exe.read_bytes()).hexdigest(),
              'reason':'startup probe cannot establish same-action gameplay equivalence'}
    required = ['wine', 'wineserver', 'Xephyr', 'xdotool', 'xwd']
    missing = [tool for tool in required if not shutil.which(tool)]
    if missing or not os.environ.get('DISPLAY'):
        result['reason'] = 'original runtime prerequisites unavailable: ' + ','.join(missing or ['DISPLAY'])
        (output/'probe.json').write_text(json.dumps(result,indent=2))
        return result
    # Xephyr -displayfd allocates an unused display; never touches user's WM.
    read_fd, write_fd = os.pipe()
    processes = []
    env = dict(os.environ, WINEPREFIX=str(output/'wine32'), WINEARCH='win32',
               WINEDEBUG='-all', WINEDLLOVERRIDES='mscoree,mshtml=')
    try:
        with (output/'xephyr.log').open('w') as log:
            server = subprocess.Popen(['Xephyr','-displayfd',str(write_fd),'-screen','800x600',
                                       '-nolisten','tcp','-noreset'],pass_fds=(write_fd,),
                                      stdout=log,stderr=log)
        processes.append(server)
        os.close(write_fd)
        import select
        if not select.select([read_fd], [], [], 10)[0]:
            raise RuntimeError('nested X server startup timed out')
        env['DISPLAY'] = ':' + os.read(read_fd,64).decode().strip()
        with (output/'wineboot.log').open('w') as log:
            subprocess.run(['wine','wineboot','--init'],env=env,stdout=log,stderr=log,timeout=45)
        prefix = output/'wine32'
        (prefix/'dosdevices/z:').unlink(missing_ok=True)
        game = prefix/'drive_c/game'
        game.mkdir()
        # The extracted port root intentionally omits installation files such
        # as drives.btb, cursors and Title.avi. Use the authentic installation
        # tree for the original runtime; otherwise its unchecked fopen fails.
        install = None
        volumes = {}
        if iso:
            from media_sources import discover
            for root in (iso.resolve().parent, iso.resolve().parent.parent):
                try:
                    volumes = discover(root)
                    install = volumes[1]
                    break
                except ValueError:
                    continue
        cache=output.parent/'original-media'
        if install:
            installation=prepare_volume(install,cache)
            shutil.copytree(installation/'copy2hd',game,dirs_exist_ok=True)
            result['installation_iso']=str(install)
        else:
            for name in ('engine','fonts','savegame','goldsave','setupart'):
                shutil.copytree(data/'copy2hd'/name,game/name)
        shutil.copy2(exe,game/'BETRAYAL.EXE')
        for name in ('anims','backs','nodes','inventor','sounds','dialog','video','avi','cursors'):
            if not (game/name).exists() and (data/name).exists():
                (game/name).symlink_to(data/name,target_is_directory=True)
        if iso:
            if not volumes:
                from media_sources import disc_number
                volumes={disc_number(iso):iso.resolve()}
            result['mapped_discs']={}
            for index,(number,source) in enumerate(sorted(volumes.items())):
                letter=chr(ord('d')+index)
                disc=prepare_volume(source,cache)
                (prefix/('dosdevices/'+letter+':')).symlink_to(disc,target_is_directory=True)
                # Directory mapping suffices; no host block device or mounting.
                subprocess.run(['wine','reg','add',r'HKLM\Software\Wine\Drives','/v',letter+':',
                                '/t','REG_SZ','/d','cdrom','/f'],env=env,timeout=15,check=True,
                               stdout=subprocess.DEVNULL,stderr=subprocess.DEVNULL)
                result['mapped_discs'][str(number)]={'drive':letter.upper()+':','iso':str(source)}
        # mountmgr reads drive type from HKLM when the private server starts.
        subprocess.run(['wineserver','-k'],env=env,timeout=10,check=True)
        subprocess.run(['wineserver','-w'],env=env,timeout=10,check=True)
        with (output/'original.log').open('w') as log:
            original = subprocess.Popen(['wine','explorer','/desktop=ByzantineReference,800x600',
                                         r'C:\game\BETRAYAL.EXE'],cwd=game,env=env,stdout=log,stderr=log)
        processes.append(original)
        time.sleep(8)
        with (output/'startup.xwd').open('wb') as capture:
            subprocess.run(['xwd','-root','-silent'],env=env,stdout=capture,check=True,timeout=10)
        windows = subprocess.run(['xdotool','search','--name','.*'],env=env,capture_output=True,text=True,timeout=10)
        result['windows'] = windows.stdout.splitlines()
        result['exit_code'] = original.poll()
        result['capture'] = 'startup.xwd'
        xwd_png(output/'startup.xwd',output/'startup.png')
        result['preview'] = 'startup.png'
        result['disc_iso'] = str(iso) if iso else None
    except Exception as exc:
        result['reason'] = 'original startup probe: ' + str(exc)
    finally:
        os.close(read_fd)
        try: subprocess.run(['wineserver','-k'],env=env,timeout=10,stdout=subprocess.DEVNULL,stderr=subprocess.DEVNULL)
        except Exception: pass
        for process in reversed(processes):
            if process.poll() is None:
                process.terminate()
                try: process.wait(timeout=5)
                except subprocess.TimeoutExpired: process.kill(); process.wait()
    (output/'probe.json').write_text(json.dumps(result,indent=2)+'\n')
    return result


if __name__ == '__main__':
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--data',type=Path,required=True)
    p.add_argument('--exe',type=Path,required=True)
    p.add_argument('--iso',type=Path)
    p.add_argument('--output',type=Path,required=True)
    a=p.parse_args()
    print(json.dumps(probe(a.data.resolve(),a.exe.resolve(),a.iso,a.output),indent=2))
