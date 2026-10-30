#!/usr/bin/env python3
"""Locate authentic Byzantine discs by CD.BYZ, independent of ISO filenames."""
import argparse
import json
import os
from pathlib import Path
import subprocess
import tempfile


def atomic_write(path, value):
    path = Path(path)
    path.parent.mkdir(parents=True, exist_ok=True)
    fd, temporary = tempfile.mkstemp(prefix='.'+path.name, dir=path.parent)
    try:
        with os.fdopen(fd, 'w') as stream:
            stream.write(value)
        os.replace(temporary, path)
    finally:
        if os.path.exists(temporary): os.unlink(temporary)


def disc_number(iso):
    result = subprocess.run(['7z','x','-so','-ssc-',str(iso),'CD.BYZ'],
                            capture_output=True, timeout=30)
    marker = result.stdout.strip()
    if result.returncode or marker not in (b'1',b'2',b'3',b'4',b'5',b'6'):
        raise ValueError(f'Byzantine CD.BYZ marker not found or invalid: {iso}')
    return int(marker)


def discover(root, reader=disc_number):
    root = Path(root).resolve()
    if not root.is_dir(): raise ValueError(f'ISO directory does not exist: {root}')
    candidates = list(root.iterdir())
    for child in list(candidates):
        if child.is_dir() and not child.is_symlink(): candidates.extend(child.iterdir())
    discs = {}
    for path in sorted(candidates):
        if not path.is_file() or path.suffix.lower() != '.iso': continue
        path = path.resolve()
        if '\n' in str(path) or '\r' in str(path): raise ValueError('ISO path contains newline')
        try: disc = reader(path)
        except ValueError: continue  # unrelated ISOs may share the directory
        if disc in discs and discs[disc] != path:
            raise ValueError(f'Ambiguous disc {disc}: {discs[disc]} and {path}')
        discs[disc] = path
    missing = sorted(set(range(1,7))-set(discs))
    if missing: raise ValueError(f'Missing Byzantine ISO disc(s) {missing} in {root}; original filenames are not required')
    return discs


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--root',type=Path)
    parser.add_argument('--config',type=Path,required=True)
    parser.add_argument('--output',type=Path,required=True)
    parser.add_argument('--disc',type=int,choices=range(1,7), help='validate a single manifest member; print its ISO')
    args=parser.parse_args()
    try:
        if args.disc:
            manifest=json.loads(args.config.read_text())
            path=Path(manifest['discs'][str(args.disc)]['path'])
            if disc_number(path)!=args.disc: raise ValueError(f'Wrong ISO for requested disc {args.disc}: {path}')
            print(path)
            return
        root=args.root
        if root is None:
            if args.config.is_file(): root=Path(json.loads(args.config.read_text())['root'])
            else: root=Path(__file__).resolve().parent.parent
        discs=discover(root)
        manifest={'version':1,'root':str(root.resolve()),'discs':{str(n):{
            'path':str(p),'size':p.stat().st_size,'mtime_ns':p.stat().st_mtime_ns}
            for n,p in discs.items()}}
        atomic_write(args.output, ''.join(str(discs[n])+'\n' for n in range(1,7)))
        atomic_write(args.config,json.dumps(manifest,indent=2)+'\n')
        print('\n'.join(str(discs[n]) for n in range(1,7)))
    except (ValueError,OSError,KeyError,subprocess.SubprocessError,json.JSONDecodeError) as error:
        parser.exit(1,f'Media setup: {error}\n')

if __name__=='__main__': main()
