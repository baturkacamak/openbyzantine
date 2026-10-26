#!/usr/bin/env python3
"""Verify saved exports, focused assembly and original PE byte ranges."""
import hashlib
import json
import struct
from pathlib import Path


def digest(data):
    return hashlib.sha256(data).hexdigest()


def main():
    here = Path(__file__).resolve().parent
    evidence = json.loads((here / 'panorama-projection-evidence.json').read_text())
    index = json.loads((here / 'pseudocode/decompile-index.json').read_text())
    binary = (here / 'BETRAYAL.EXE').read_bytes()
    if digest(binary) != index['target_sha256'] or digest(binary) != evidence['target_sha256']:
        raise ValueError('target identity mismatch')
    if evidence['analysis_profile_digest'] != index['analysis_profile_digest']:
        raise ValueError('analysis profile mismatch')
    entries = {item['address']: item for item in index['functions']}
    for source in evidence['sources']:
        if any(source[key] != entries[source['address']][key] for key in source):
            raise ValueError(f"export index mismatch: {source['address']}")
        if digest((here / 'pseudocode' / source['relative_path']).read_bytes()) != source['sha256']:
            raise ValueError(f"export hash mismatch: {source['address']}")
    # Resolve each address independently through the PE section table.
    pe = struct.unpack_from('<I', binary, 0x3c)[0]
    count = struct.unpack_from('<H', binary, pe + 6)[0]
    optional_size = struct.unpack_from('<H', binary, pe + 20)[0]
    base = struct.unpack_from('<I', binary, pe + 24 + 28)[0]
    sections = []
    for i in range(count):
        pos = pe + 24 + optional_size + i * 40
        _, rva, size, offset = struct.unpack_from('<IIII', binary, pos + 8)
        sections.append((base + rva, size, offset))

    def check_range(address, size, offset, expected):
        mappings = [raw + address - start for start, length, raw in sections
                    if start <= address and address + size <= start + length]
        if mappings != [offset] or digest(binary[offset:offset + size]) != expected:
            raise ValueError(f'PE byte range mismatch: {address:#x}')

    for item in evidence['disassembly']:
        start = int(item['start'], 16)
        size = int(item['end_exclusive'], 16) - start
        check_range(start, size, item['file_offset'], item['byte_sha256'])
        if digest((here / item['relative_path']).read_bytes()) != item['sha256']:
            raise ValueError('retained disassembly mismatch')
    for item in evidence['data']:
        check_range(int(item['address'], 16), item['size'], item['file_offset'], item['sha256'])
        offset = item['file_offset']
        if binary[offset:offset + item['size']].hex() != item['hex']:
            raise ValueError('retained data mismatch')
    print(f"Target/profile, {len(evidence['sources'])} exports, "
          f"{len(evidence['disassembly'])} assembly ranges and "
          f"{len(evidence['data'])} data ranges verified")


if __name__ == '__main__':
    main()
