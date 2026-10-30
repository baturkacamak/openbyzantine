"""Discover genuine on-disk numbered saves without inventing entry labels/state."""
from collections import defaultdict
import hashlib
from pathlib import Path
import re
import subprocess

PORT=Path(__file__).resolve().parent


def parse_save_summary(text):
    disc=re.search(r'^cd=([1-6])\b',text,re.MULTILINE)
    scene=re.search(r'^scene_index=(\d+) chapter_scenes=(\d+) scene=([A-Za-z0-9_]+)\b',text,re.MULTILINE)
    if not disc or not scene or not 0<=int(scene[1])<int(scene[2]):
        raise ValueError('save has no valid disc/scene metadata')
    return {'scene':scene[3].upper(),'disc':int(disc[1])}


def read_save_metadata(data, path):
    result=subprocess.run([str(PORT/'byzantine-cc'),'--save-dump',str(path),
                           '--lcc',str(data/'copy2hd/engine/SHOW.LCC')],
                          capture_output=True,timeout=15)
    if result.returncode:
        raise ValueError(result.stderr.decode(errors='replace')[-500:])
    # Descriptions/record names may contain original CP1254 bytes. Metadata
    # labels are ASCII; display-text decoding must not reject a valid save.
    return parse_save_summary(result.stdout.decode(errors='replace'))


def collect_save_seeds(data, reader=None):
    data=Path(data)
    read=reader or (lambda path:read_save_metadata(data,path))
    candidates=sorted(p for p in (data/'copy2hd/savegame').iterdir()
                      if p.is_file() and re.fullmatch(r'\d\d\.btb',p.name,re.IGNORECASE))
    groups=defaultdict(list);seen={};skipped=[]
    for path in candidates:
        try:
            metadata=read(path)
            sha=hashlib.sha256(path.read_bytes()).hexdigest()
            if sha in seen:
                skipped.append({'save':path.name,'reason':'duplicate bytes','duplicate_of':seen[sha]})
                continue
            spec={'id':f'save_{path.stem}_{metadata["scene"].lower()}',
                  'scene':metadata['scene'],'save':path.name,'label':0,'restore':True}
            groups[spec['scene']].append(spec);seen[sha]=path.name
        except (ValueError,OSError,subprocess.SubprocessError) as error:
            skipped.append({'save':path.name,'reason':str(error)})
    # Cover different scenes before returning to another save of the same one.
    seeds=[]
    for index in range(max((len(rows) for rows in groups.values()),default=0)):
        for scene in sorted(groups):
            if index<len(groups[scene]):seeds.append(groups[scene][index])
    catalogue={'mode':'numbered-saves','source':str((data/'copy2hd/savegame').resolve()),
               'candidate_count':len(candidates),'seed_count':len(seeds),
               'scenes':sorted(groups),'skipped':skipped,'ordering':'scene-round-robin'}
    if not seeds:raise ValueError('no usable numbered save contexts: '+str(skipped))
    return seeds,catalogue
