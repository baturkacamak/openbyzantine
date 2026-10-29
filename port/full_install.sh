#!/usr/bin/env bash
# Copy every dialogue/movie file from the six discs into a prepared data dir.
#
#   ./port/full_install.sh "<game-dir>" [data-dir]
#
# The normal player extracts a missing conversation clip from the ISO at the
# moment it is needed. After this script the player no longer needs the discs
# for playback (the launcher's --media-isos manifest can stay in place; it simply
# finds the files already on disk). This is also the prerequisite for building
# per-language voice overlays without repeated ISO extraction.
set -euo pipefail

here="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
game_dir="${1:?usage: $0 <game-dir> [data-dir]}"
data="${2:-${BYZ_DATA:-${TMPDIR:-/tmp}/byz-scene}}"

command -v 7z >/dev/null 2>&1 || { echo "7z is required" >&2; exit 1; }
[ -d "$data" ] || { echo "prepared data dir not found: $data (run scene.sh once first)" >&2; exit 1; }

find_iso() {
    local index="$1" candidate
    for candidate in \
        "$game_dir/Byzantine-The-Betrayal_Win_TR_CD$index/Byzantine_$index.iso" \
        "$game_dir"/*"CD$index"*/Byzantine_"$index".iso \
        "$game_dir"/Byzantine_"$index".iso; do
        [ -f "$candidate" ] && { printf '%s' "$candidate"; return 0; }
    done
    return 1
}

copied=0
for index in 1 2 3 4 5 6; do
    iso="$(find_iso "$index")" || { echo "missing disc $index; skipped" >&2; continue; }
    echo "extracting dialogs/movies from disc $index ..."
    7z x -y "$iso" "dialog/*" "video/*" "avi/*" -o"$data" >/dev/null 2>&1 || true
    copied=$((copied + 1))
done
touch "$data/.full_install"
echo "full install complete: $copied disc(s), data=$data"
