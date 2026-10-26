#!/usr/bin/env python3
"""Validate the exact saved static sources used for the camera/seam recovery."""
import hashlib
import json
from pathlib import Path


def main():
    here = Path(__file__).resolve().parent
    index = json.loads((here / 'pseudocode/decompile-index.json').read_text())
    evidence = json.loads((here / 'panorama-camera-evidence.json').read_text())
    target = hashlib.sha256((here / 'BETRAYAL.EXE').read_bytes()).hexdigest()
    if target != index['target_sha256'] or target != evidence['target_sha256']:
        raise ValueError('target identity mismatch')
    if evidence['analysis_profile_digest'] != index['analysis_profile_digest']:
        raise ValueError('analysis profile mismatch')
    entries = {item['address']: item for item in index['functions']}
    for source in evidence['sources']:
        entry = entries[source['address']]
        if any(source[key] != entry[key] for key in source):
            raise ValueError(f"index mismatch: {source['address']}")
        digest = hashlib.sha256((here / 'pseudocode' / source['relative_path']).read_bytes()).hexdigest()
        if digest != source['sha256']:
            raise ValueError(f"export hash mismatch: {source['address']}")
    print(f"Target/profile and {len(evidence['sources'])} camera/projection exports verified")


if __name__ == '__main__':
    main()
