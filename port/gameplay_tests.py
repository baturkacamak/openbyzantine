#!/usr/bin/env python3
"""Asset-backed SDL regression suite. No game data or references are generated as truth."""
import argparse
import hashlib
import json
import os
import re
from pathlib import Path
import shutil
import subprocess
import time

PORT = Path(__file__).resolve().parent
DURABLE = ('scene', 'variable_pool', 'inventory', 'channels', 'conversation')


def digest(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def execute(command, log, timeout, env):
    with Path(log).open('w') as output:
        try:
            result = subprocess.run(command, stdout=output, stderr=subprocess.STDOUT,
                                    timeout=timeout, env=env, cwd=PORT)
        except subprocess.TimeoutExpired as exc:
            raise AssertionError(f'player timed out after {timeout}s; see {log}') from exc
    text = Path(log).read_text(errors='replace')
    if result.returncode:
        raise AssertionError(f'player exit {result.returncode}; see {log}')
    if 'automation_complete actions=' not in text:
        raise AssertionError(f'incomplete input script; see {log}')
    return text


def check(snapshot, expected):
    for key, value in expected.items():
        if snapshot.get(key) != value:
            raise AssertionError(f'{key}: expected {value!r}, got {snapshot.get(key)!r}')
    if snapshot.get('session_ended'):
        raise AssertionError('unexpected ended session')


def compare(left, right, fields):
    different = [key for key in fields if key not in left or key not in right or left[key] != right[key]]
    if different:
        raise AssertionError('state differs: ' + ', '.join(different))


def compare_original(reference, case, checkpoints, target_hash, script_hash, save_hash):
    """Only independently supplied original captures may serve as an oracle."""
    if reference is None or not (reference / (case + '.json')).is_file():
        return {'status': 'UNVERIFIED', 'reason': 'original runtime checkpoints unavailable'}
    manifest = json.loads((reference / (case + '.json')).read_text())
    for key, value in {'producer': 'original-runtime', 'exe_sha256': target_hash,
                       'script_sha256': script_hash, 'save_sha256': save_hash}.items():
        if manifest.get(key) != value:
            raise AssertionError('original reference provenance mismatch: ' + key)
    expected_names = {p.stem for p in checkpoints.glob('*.json')}
    if set(manifest['checkpoints']) != expected_names:
        raise AssertionError('original reference must cover every script checkpoint')
    count = 0
    for name, entry in manifest['checkpoints'].items():
        # References are actual original RGB PPM captures at 640x480, with a
        # declared tolerance. No rescaling or blanket ignored pixels.
        candidate = checkpoints / (name + '.ppm')
        source = (reference / entry['image']).resolve()
        if reference.resolve() not in source.parents:
            raise AssertionError('reference image escapes reference directory')
        if digest(source) != entry['sha256']:
            raise AssertionError('reference image hash mismatch')
        a, b = ppm(source), ppm(candidate)
        if len(a) != len(b):
            raise AssertionError('original image dimensions differ')
        error = sum(abs(x-y) for x,y in zip(a,b)) / len(a)
        tolerance = float(entry.get('mean_absolute_error', 0))
        if tolerance < 0 or tolerance > 16:
            raise AssertionError('original tolerance must be within 0..16')
        if error > tolerance:
            raise AssertionError(f'original {name}: pixel MAE {error:.3f} > {tolerance}')
        count += 1
    if not count:
        raise AssertionError('original reference contains no checkpoints')
    return {'status': 'PASS', 'checkpoints': count}


def ppm(path):
    # Strict capture contract avoids interpreting arbitrary headers as pixels.
    raw = Path(path).read_bytes()
    header = b'P6\n640 480\n255\n'
    if not raw.startswith(header) or len(raw) != len(header) + 640*480*3:
        raise AssertionError('expected binary 640x480 RGB PPM: ' + str(path))
    return raw[len(header):]


def prepare(data, output):
    engine = data / 'copy2hd/engine'
    tables = output / 'tables'
    tables.mkdir()
    for archive in ('CC', 'CB', 'CO', 'CI', 'CY', 'CF', 'CS', 'CA', 'CD', 'CE', 'CCB', 'CCF', 'Byz'):
        source = engine / (archive + ('.all' if archive == 'Byz' else '.ALL'))
        subprocess.run([str(PORT / 'byzantine-all'), str(source), str(tables / archive.lower())],
                       check=True, stdout=subprocess.DEVNULL)
    return engine, tables


def run_case(case, args, engine, tables, output):
    folder = output / case['name']
    folder.mkdir()
    saves = folder / 'saves'
    shutil.copytree(args.data / 'copy2hd/savegame', saves)
    save = saves / case['save']
    prefs = next(p for p in (saves/'gamepref_000', saves/'GAMEPREF.000', args.data/'copy2hd/goldsave/GAMEPREF.000') if p.is_file())
    shutil.copy2(prefs, folder/'GAMEPREF.000')
    actions = list(case['actions'])
    for iteration in range(case.get('repeat', 0) * args.soak_multiplier):
        actions.extend(x.replace('{i}', str(iteration)) for x in case['loop'])
    actions += ['quit']
    script = folder / 'actions.txt'
    script.write_text('\n'.join(actions) + '\n')
    checkpoints = folder / 'checkpoints'
    scene = case['scene']
    command = [str(PORT / 'byzantine-play'), str(tables/'cc'/scene), str(tables/'cb'/scene),
               str(case['label']), '--save', str(save), '--engine', str(engine),
               '--config', str(folder/'settings.ini'), '--no-audio', '--no-video', '--no-transitions',
               '--automation', str(script), '--automation-output', str(checkpoints),
               '--gameprefs', str(folder/'GAMEPREF.000'), '--backgrounds', str(args.data), '--anims', str(args.data),
               '--sounds', str(args.data), '--fonts', str(args.data/'copy2hd/fonts'),
               '--messages', str(args.data/'copy2hd/setupart/MESSAGES.TXT'),
               '--inventor', str(args.data/'inventor.txt'), '--hotitems', str(args.data/'hotitems.txt'),
               '--ccb', str(tables/'ccb'), '--ccf', str(tables/'ccf'), '--byz', str(tables/'byz'),
               '--dialog', str(args.data/'dialog'), '--dialog-cache', str(folder/'dialog-cache'),
               '--conversation-choices', '0,0,0,0,0,0,0,0']
    for table in ('ci','cy','cf','cs','ca','co','cd','ce'):
        command += ['--'+table, str(tables/table/scene)]
    for table in ('lcx','lci','lcy'):
        command += ['--'+table, str(engine/('SHOW.'+table.upper()))]
    (folder/'command.json').write_text(json.dumps(command, indent=2))
    save_hash = digest(save)
    log = execute(command, folder/'player.log', args.timeout,
            dict(os.environ, SDL_VIDEODRIVER='dummy', SDL_AUDIODRIVER='dummy'))
    for required in case.get('required_log', []):
        if required not in log:
            raise AssertionError('required action did not commit: ' + required)
    if re.search(r'^click .* stopped:', log, re.MULTILINE):
        raise AssertionError('committed click blocked; see player.log')
    states = {p.stem: json.loads(p.read_text()) for p in checkpoints.glob('*.json')}
    for name, expected in case.get('expect', {}).items():
        check(states[name], expected)
    for iteration in range(case.get('repeat',0) * args.soak_multiplier):
        check(states['inside_'+str(iteration)], case['inside_expect'])
        check(states['cycle_'+str(iteration)], case['loop_expect'])
    for assertion in case.get('equal', []):
        compare(states[assertion[0]], states[assertion[1]], assertion[2])
    result = {'name': case['name'], 'category': case['category'], 'status': 'PASS',
              'script_sha256': digest(script), 'save_sha256': save_hash,
              'checkpoints': len(states),
              'diagnostics': [line for line in log.splitlines() if ' missing' in line or 'skipped (' in line or 'reason=--no-' in line],
              'media': 'disabled; logic/input checks only'}
    try:
        result['original'] = compare_original(args.original_references, case['name'], checkpoints,
                                              args.exe_hash, digest(script), save_hash)
    except Exception as exc:
        result['original'] = {'status': 'FAIL', 'reason': str(exc)}
    return result


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--data', type=Path, required=True, help='scene.sh extracted data root')
    parser.add_argument('--output', type=Path)
    parser.add_argument('--timeout', type=float, default=120)
    parser.add_argument('--soak-multiplier', type=int, default=1)
    parser.add_argument('--original-iso', type=Path, help='read-only play-disc ISO for private Wine startup probe')
    parser.add_argument('--original-references', type=Path)
    parser.add_argument('--allow-unverified-original', action='store_true',
                        help='allow logic tests to succeed while original remains explicitly UNVERIFIED')
    args = parser.parse_args()
    args.data = args.data.resolve()
    if args.soak_multiplier < 1 or args.timeout <= 0:
        parser.error('positive timeout and soak multiplier required')
    output = (args.output or PORT/'output'/('gameplay-'+time.strftime('%Y%m%dT%H%M%S'))).resolve()
    output.mkdir(parents=True, exist_ok=False)
    exe = PORT.parent/'analysis/rea/BETRAYAL.EXE'
    args.exe_hash = digest(exe)
    index = json.loads((PORT.parent/'analysis/rea/pseudocode/decompile-index.json').read_text())
    if args.exe_hash != index['target_sha256']:
        raise AssertionError('original EXE hash differs from decompile index')
    # Target identity is also retained in the report; no new binary behavior is derived here.
    report = {'schema': 1, 'exe_sha256': args.exe_hash, 'player_sha256': digest(PORT/'byzantine-play'),
              'data': str(args.data), 'engine_archives': {p.name:digest(p) for p in (args.data/'copy2hd/engine').iterdir() if p.is_file()}, 'cases': [], 'scope': 'dummy SDL; no physical input/audio/monitor validation'}
    if not args.original_iso and not args.original_references:
        candidate = PORT.parent/'Byzantine-The-Betrayal_Win_TR_CD2/Byzantine_2.iso'
        if candidate.is_file(): args.original_iso = candidate
    if args.original_iso:
        from original_runtime_probe import probe
        report['original_startup'] = probe(args.data, exe, args.original_iso, output/'original-startup')
    engine, tables = prepare(args.data, output)
    cases = json.loads((PORT/'tests/gameplay/scenarios.json').read_text())
    for case in cases:
        try:
            result = run_case(case, args, engine, tables, output)
        except Exception as exc:
            result = {'name': case['name'], 'category': case['category'], 'status': 'FAIL', 'reason': str(exc)}
        report['cases'].append(result)
        print(result['name'], result['status'], result.get('reason',''), flush=True)
    failed = any(c['status'] != 'PASS' or c.get('original',{}).get('status') == 'FAIL' for c in report['cases'])
    unverified = any(c.get('original',{}).get('status') != 'PASS' for c in report['cases'])
    report['categories'] = {category: ('PASS' if all(c['status']=='PASS' for c in report['cases'] if c['category']==category) else 'FAIL')
                            for category in ('action-chains','save-load','soak')}
    report['categories']['original-comparison'] = 'UNVERIFIED' if unverified else 'PASS'
    if any(c.get('original',{}).get('status')=='FAIL' for c in report['cases']):
        report['categories']['original-comparison'] = 'FAIL'
    report['status'] = 'FAIL' if failed else ('UNVERIFIED' if unverified else 'PASS')
    (output/'report.json').write_text(json.dumps(report, indent=2)+'\n')
    print('Report:', output/'report.json', report['status'])
    return 1 if failed or (unverified and not args.allow_unverified_original) else 0


if __name__ == '__main__':
    raise SystemExit(main())
