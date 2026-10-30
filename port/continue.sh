#!/usr/bin/env bash
# Resume the port's automatic save (`savegame/autosave-port.btb`).
#
#   ./port/continue.sh "<game-dir>" [extra scene.sh args...]
#
# The autosave is written after every scene travel when the port's "Oto kayit"
# setting is on. This wrapper reads its scene index/name with byzantine-cc and
# launches scene.sh with that save, so the normal startup state is restored.
set -euo pipefail

here="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
game_dir="${1:?usage: $0 <game-dir> [scene.sh args...]}"
shift || true
data="${BYZ_DATA:-${TMPDIR:-/tmp}/byz-scene}"
save="$data/copy2hd/savegame/autosave-port.btb"

[ -f "$save" ] || { echo "no autosave: $save (play once with Oto kayit on)" >&2; exit 1; }
lcc="$data/copy2hd/engine/SHOW.LCC"
[ -f "$lcc" ] || { echo "no scene table: $lcc" >&2; exit 1; }

scene="$("$here/byzantine-cc" --save-dump "$save" --lcc "$lcc" 2>/dev/null |
    sed -n 's/.* scene=\([^ ]*\).*/\1/p' | awk 'NR==1{print}')"
[ -n "$scene" ] || { echo "cannot read the autosave scene" >&2; exit 1; }
upper="$(printf '%s' "$scene" | tr '[:lower:]' '[:upper:]')"
[ -f "$here/output/cc_all/$upper" ] || {
    echo "autosave scene '$upper' has no extracted script" >&2
    exit 1
}

echo "continue scene=$upper save=$save"
exec "$here/scene.sh" "$game_dir" "$upper" 0 "autosave-port.btb" --restore-save "$@"
