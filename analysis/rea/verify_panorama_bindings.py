#!/usr/bin/env python3
"""Reuse saved REA exports and inspect static panorama bindings; never run EXE."""
import hashlib
import json
from pathlib import Path
import struct
import subprocess

ROOT = Path(__file__).resolve().parents[2]
HERE = Path(__file__).resolve().parent


def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def run(*args):
    return subprocess.check_output([str(x) for x in args], text=True)


def main():
    index = json.loads((HERE / 'pseudocode/decompile-index.json').read_text())
    assert digest(HERE / 'BETRAYAL.EXE') == index['target_sha256']
    addresses = {'0x42db22', '0x43340a', '0x432295', '0x40186e', '0x401c21',
                 '0x401ddf', '0x412d35', '0x412f77', '0x411dfc', '0x436652'}
    sources = []
    for item in index['functions']:
        if item['address'] in addresses:
            path = HERE / 'pseudocode' / item['relative_path']
            assert digest(path) == item['sha256'], path
            sources.append({k: item[k] for k in ('address', 'relative_path', 'sha256')})
    assert len(sources) == len(addresses)
    # Read the PE's section map, without executing or importing the binary.
    data = (HERE / 'BETRAYAL.EXE').read_bytes()
    pe = struct.unpack_from('<I', data, 60)[0]
    count = struct.unpack_from('<H', data, pe + 6)[0]
    optional_size = struct.unpack_from('<H', data, pe + 20)[0]
    base = struct.unpack_from('<I', data, pe + 52)[0]
    strings = {}
    for address in (0x45eb10, 0x45c0b8, 0x45c0c0):
        for i in range(count):
            section = pe + 24 + optional_size + 40 * i
            _, va, size, offset = struct.unpack_from('<IIII', data, section + 8)
            if va <= address - base < va + size:
                start = offset + address - base - va
                strings[hex(address)] = data[start:data.index(0, start)].decode('ascii')
                break
    assert strings == {'0x45eb10': 'co', '0x45c0b8': '%.2s\\%s',
                       '0x45c0c0': 'nodes\\%s.jpg'}
    inventory = {}
    for iso in sorted(ROOT.glob('Byzantine-The-Betrayal_Win_TR_CD*/*.iso')):
        listing = run('7z', 'l', '-slt', iso)
        for line in listing.splitlines():
            if line.startswith('Path = '):
                path = line[7:].replace('\\', '/')
                key = path.lower().removeprefix('copy2hd/')
                if key.startswith('nodes/') and key.endswith('.jpg'):
                    inventory.setdefault(key, []).append({'iso': str(iso.relative_to(ROOT)),
                                                         'member': path})
    scenes = []
    for co in sorted((ROOT / 'port/output/co_all').iterdir()):
        output = run(ROOT / 'port/byzantine-cc', ROOT / 'port/output/cc_all' / co.name,
                     '--panoramas', co)
        commands = []
        for line in output.splitlines():
            row = dict(field.split('=', 1) for field in line.split())
            for field in ('instruction', 'id'):
                row[field] = int(row[field])
            row['assets'] = inventory.get(row['jpeg'].lower(), [])
            commands.append(row)
        scenes.append({'scene': co.name, 'co_sha256': digest(co), 'commands': commands})
    report = {'authority': 'saved REA/Ghidra static export + original archive inventory',
              'target_sha256': index['target_sha256'],
              'analysis_profile_digest': index['analysis_profile_digest'],
              'sources': sources, 'pe_strings': strings, 'scenes': scenes,
              'limits': ['No original game execution or runtime path reachability observed.',
                         'Case-insensitive ISO filename matching; no panorama projection implemented.',
                         'CO parser accepts the observed 0xff flag records only.']}
    report['summary'] = {'scenes': len(scenes),
                         'load_commands': sum(len(s['commands']) for s in scenes),
                         'missing_commands': sum(not c['assets'] for s in scenes for c in s['commands'])}
    (HERE / 'panorama-binding-evidence.json').write_text(json.dumps(report, indent=2) + '\n')
    print(json.dumps(report['summary']))


if __name__ == '__main__':
    main()
