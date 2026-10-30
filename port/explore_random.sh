#!/usr/bin/env bash
# One-command random exploration. Does not launch or overwrite normal game logs.
set -euo pipefail
here="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
count="${1:-2000}"
if [ "$#" -gt 0 ]; then shift; fi
if [ "$#" -gt 0 ]; then
    seed="$1"
    shift
else
    seed="$(python3 -c 'import secrets; print(secrets.randbits(32))')"
fi
if ! [[ "$count" =~ ^[1-9][0-9]*$ && "$seed" =~ ^[0-9]+$ ]]; then
    printf 'Kullanım: %s [deneme-sayısı] [rastgelelik-tohumu] [ek keşif seçenekleri]\n' "$0" >&2
    exit 2
fi
for argument in "$@"; do
    case "$argument" in
        --output|--output=*|--replay|--replay=*|--strategy|--strategy=*|--random-seed|--random-seed=*)
            printf 'Bu seçenek için doğrudan explore_game.py kullanın: %s\n' "$argument" >&2
            exit 2 ;;
    esac
done
data="${BYZ_DATA:-${TMPDIR:-/tmp}/byz-scene}"
if [ ! -f "$data/copy2hd/engine/CC.ALL" ]; then
    printf 'Hazırlanmış oyun verisi bulunamadı: %s\nÖnce port/scene.sh ile verileri hazırlayın veya BYZ_DATA belirtin.\n' "$data" >&2
    exit 2
fi
make -C "$here" -j2 all
mkdir -p "$here/output"
output="$here/output/random-$(date -u +%Y%m%dT%H%M%SZ)-$$"
log="$output.log"
printf 'Karma tarama: en fazla %s çalıştırma; tohum=%s\nRapor dizini: %s\n' "$count" "$seed" "$output"
# Separate from output/logs/LATEST; points to the latest exploration attempt,
# including while it is still running. Startup output initially uses <run>.log.
printf '%s\n' "$output" > "$here/output/EXPLORATION_LATEST.tmp.$$"
mv "$here/output/EXPLORATION_LATEST.tmp.$$" "$here/output/EXPLORATION_LATEST"
seed_options=(--all-saves)
for argument in "$@"; do
    case "$argument" in --seeds|--seeds=*) seed_options=() ;; esac
done
set +e
python3 -u "$here/explore_game.py" --data "$data" --strategy mixed --random-seed "$seed" "${seed_options[@]}" \
    --max-runs "$count" --max-states "$count" --max-depth 24 --max-frontier 20000 \
    --max-actions 32 --max-decisions 64 --save-every 20 --timeout 60 --max-seconds 14400 \
    --screenshots findings --output "$output" "$@" 2>&1 | tee "$log"
statuses=("${PIPESTATUS[@]}")
status="${statuses[0]}"
if [ "$status" -eq 0 ] && [ "${statuses[1]}" -ne 0 ]; then status="${statuses[1]}"; fi
set -e
if [ -d "$output" ]; then
    mv "$log" "$output/runner.log"
fi
if [ -f "$output/report.json" ]; then
    python3 - "$output/report.json" <<'PY'
import json,sys
from pathlib import Path
p=Path(sys.argv[1]);r=json.loads(p.read_text())
print(f"Durum: {r['status']} · Çalıştırma: {r['runs']} · Ayrıntılı durum: {len(r['states'])} · İlerleme/bağlam: {len(r.get('progress_states', []))} · Sahne: {len(r['scenes'])} · Bulgu: {len(r['findings'])}")
print('Durma nedeni:',r.get('stop_reason',r.get('error','bilinmiyor')))
print('Rapor:',p.parent/'index.html')
print('Günlük:',p.parent/'runner.log')
PY
else
    printf 'Tarama başlatılamadı. Günlük: %s\n' "$log" >&2
fi
exit "$status"
