#!/usr/bin/env bash
# Opens the SDL2 scene player on the shipped data.
#
#   ./port/scene.sh <game-dir> [scene] [label] [save]
#
#   ./port/scene.sh "/media/Backup/Byzantine - The Betrayal"           # MAP label 87
#   ./port/scene.sh "/media/Backup/Byzantine - The Betrayal" VT 497 72.btb
#
# <game-dir> is the folder that holds the six
# "Byzantine-The-Betrayal_Win_TR_CD*" directories (or the ISOs directly).
# The first run extracts the data into $BYZ_DATA (default /tmp/byz-scene) and
# splits the per-scene engine tables; later runs reuse it.
#
# In the window: click a hotspot, H toggles the hotspot outlines, Esc quits.
set -euo pipefail

here="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

usage() {
    echo "usage: $0 <game-dir> [scene] [label] [save]" >&2
    echo "  scene defaults to MAP, label to 87, save to tempsave.btb" >&2
    exit 2
}

[ $# -ge 1 ] || usage
game_dir="$1"
scene="${2:-MAP}"
label="${3:-87}"
save="${4:-tempsave.btb}"
data="${BYZ_DATA:-${TMPDIR:-/tmp}/byz-scene}"

lower() { printf '%s' "$1" | tr '[:upper:]' '[:lower:]'; }

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

command -v 7z >/dev/null 2>&1 || { echo "7z is required" >&2; exit 1; }
iso1="$(find_iso 1)" || { echo "cannot find Byzantine_1.iso under $game_dir" >&2; exit 1; }

if [ ! -f "$data/.prepared" ]; then
    echo "preparing $data (first run only) ..."
    mkdir -p "$data"
    # CD1: the engine tables, game preferences and the savegames.
    7z x -y "$iso1" "copy2hd/engine/*.ALL" "copy2hd/engine/SHOW.L*" \
        "copy2hd/goldsave/GAMEPREF.000" "copy2hd/savegame/*.btb" -o"$data" >/dev/null
    # Every CD: backgrounds, panorama nodes, animation assets, sounds.
    for index in 1 2 3 4 5 6; do
        iso="$(find_iso "$index")" || continue
        7z x -y "$iso" "backs/*" "nodes/*" "anims/*" "sounds/*" -o"$data" >/dev/null
    done
    # 0x407254 decides the inventory media from the shipped TGA names (they are
    # spread over the CDs, so the listing is the union).
    listing=""
    for index in 1 2 3 4 5 6; do
        iso="$(find_iso "$index")" || continue
        listing+="$(7z l "$iso")"$'\n'
    done
    printf '%s\n' "$listing" | awk 'NF{print tolower($NF)}' | \
        grep -E '^inventor/hotitems/.*\.tga$' | sed 's|.*/||; s|\.tga$||' | sort >"$data/hotitems.txt"
    printf '%s\n' "$listing" | awk '{print tolower($NF)}' | \
        grep -E '^inventor/[^/]*\.tga$' | sed 's|.*/||; s|\.tga$||' | sort >"$data/inventor.txt"
    # Split the per-scene engine tables (CS CI CY CF CA) the way
    # byzantine-all does: one file per record name.
    for archive in CS CI CY CF CA; do
        target="$(lower "$archive")"
        mkdir -p "$data/$target"
        "$here/byzantine-all" "$data/copy2hd/engine/$archive.ALL" "$data/$target" >/dev/null
    done
    touch "$data/.prepared"
fi

upper_scene="$(printf '%s' "$scene" | tr '[:lower:]' '[:upper:]')"
cc="$here/output/cc_all/$upper_scene"
cb="$here/output/cb_all/$upper_scene"
co="$here/output/co_all/$upper_scene"
[ -f "$cc" ] || { echo "no scene script for '$scene' ($cc)" >&2; exit 1; }

save_path="$data/copy2hd/savegame/$save"
[ -f "$save_path" ] || { echo "no savegame '$save' in $data/copy2hd/savegame" >&2; exit 1; }

exec "$here/byzantine-play" "$cc" "$cb" "$label" \
    --save "$save_path" \
    --engine "$data/copy2hd/engine" \
    --lcx "$data/copy2hd/engine/SHOW.LCX" \
    --lci "$data/copy2hd/engine/SHOW.LCI" \
    --lcy "$data/copy2hd/engine/SHOW.LCY" \
    --ci "$data/ci/$upper_scene" \
    --cy "$data/cy/$upper_scene" \
    --cf "$data/cf/$upper_scene" \
    --cs "$data/cs/$upper_scene" \
    --ca "$data/ca/$upper_scene" \
    --co "$co" \
    --anims "$data" \
    --sounds "$data" \
    --backgrounds "$data" \
    --gameprefs "$data/copy2hd/goldsave/GAMEPREF.000" \
    --inventor "$data/inventor.txt" \
    --hotitems "$data/hotitems.txt" \
    "${@:5}"
