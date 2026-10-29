#!/usr/bin/env bash
# Opens the SDL2 scene player on the shipped data.
#
#   ./port/scene.sh <game-dir> [scene] [label] [save]
#
#   ./port/scene.sh "/path/to/openbyzantine"           # MAP label 87
#   ./port/scene.sh "/path/to/openbyzantine" VT 497 72.btb
#
# <game-dir> is the folder that holds the six
# "Byzantine-The-Betrayal_Win_TR_CD*" directories (or the ISOs directly).
# The first run extracts the data into $BYZ_DATA (default /tmp/byz-scene) and
# splits the per-scene engine tables; later runs reuse it.
#
# In the window: click a hotspot, H toggles the hotspot outlines, Esc quits.
# A hotspot whose action reaches 0x73 switches scenes: the target is loaded from
# the same per-scene directories, the 3000-word variable pool is carried and the
# new scene runs from its first instruction (analysis/rea/SCENE_TRAVEL.md).
# MAP 87 -> click 47 twice (190,300) travels to MR (index 27); in MR click 38
# (589,330) to travel back to MAP (index 25).
set -euo pipefail

here="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Capture extraction/startup failures as well as the player. The marker only
# prevents recursion; run_logged.py gives every external launch a unique file.
if [ "${BYZ_LOG_ACTIVE:-0}" != 1 ]; then
    exec python3 "$here/run_logged.py" -- bash "$here/scene.sh" "$@"
fi

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

# Panorama nodes are stored inside CD1 as copy2hd/nodes/<scene>/..., while the
# player's --backgrounds root expects nodes/<scene>/... . Flatten them so a MAP
# destination panorama is not black. (Backgrounds are already at backs/... .)
extract_panorama_nodes() {
    local iso="$1" tmp="$data/.nodes-tmp"
    rm -rf "$tmp"
    7z x -y "$iso" "copy2hd/nodes/*" -o"$tmp" >/dev/null 2>&1 || true
    if [ -d "$tmp/copy2hd/nodes" ]; then
        mkdir -p "$data/nodes"
        cp -r "$tmp/copy2hd/nodes/." "$data/nodes/"
    fi
    rm -rf "$tmp"
}

# The inventory strip needs inventor/INSTRIP.TGA and the six icons, and every
# close-up needs inventor/examine/<item>.jpg. The original top-bar assets live
# under copy2hd/setupart; flatten them to setupart/ for the same lookup.
extract_ui_assets() {
    local iso="$1" tmp="$data/.ui-tmp"
    rm -rf "$tmp"
    7z x -y "$iso" "copy2hd/setupart/*" -o"$tmp" >/dev/null 2>&1 || true
    if [ -d "$tmp/copy2hd/setupart" ]; then
        mkdir -p "$data/setupart"
        cp -r "$tmp/copy2hd/setupart/." "$data/setupart/"
    fi
    rm -rf "$tmp"
    # The game's own pointer art: copy2hd/cursors/<name>.tga and the klio set
    # under copy2hd/cursors/klio (0x40dae5). Only CD1 ships them; flatten the
    # tree so the player's <backgrounds>/cursors lookup finds it.
    7z x -y "$iso" "copy2hd/cursors/*" -o"$tmp" >/dev/null 2>&1 || true
    if [ -d "$tmp/copy2hd/cursors" ]; then
        mkdir -p "$data/cursors"
        cp -r "$tmp/copy2hd/cursors/." "$data/cursors/"
    fi
    rm -rf "$tmp"
    7z x -y "$iso" "inventor/*" "inventor/examine/*" "inventor/hotitems/*" \
        -o"$data" >/dev/null 2>&1 || true
    # A few sounds (endgame/Emre2/Pxstart) live under copy2hd/sounds, not under
    # the flat sounds/ tree. Flatten them so the player's lookup finds them.
    7z x -y "$iso" "copy2hd/sounds/*" -o"$tmp" >/dev/null 2>&1 || true
    if [ -d "$tmp/copy2hd/sounds" ]; then
        mkdir -p "$data/sounds"
        cp -r "$tmp/copy2hd/sounds/." "$data/sounds/"
    fi
    rm -rf "$tmp"
}

command -v 7z >/dev/null 2>&1 || { echo "7z is required" >&2; exit 1; }
iso1="$(find_iso 1)" || { echo "cannot find Byzantine_1.iso under $game_dir" >&2; exit 1; }

if [ ! -f "$data/.prepared" ]; then
    echo "preparing $data (first run only) ..."
    mkdir -p "$data"
    # CD1: the engine tables, game preferences and the savegames.
    7z x -y "$iso1" "copy2hd/engine/*.ALL" "copy2hd/engine/SHOW.L*" \
        "copy2hd/goldsave/GAMEPREF.000" "copy2hd/savegame/*.btb" \
        "copy2hd/savegame/BTB.SAV" "copy2hd/savegame/STATUS.000" \
        "copy2hd/savegame/NEW.CAN" "copy2hd/savegame/notebook.000" \
        "copy2hd/savegame/GAMEPREF.000" \
        "copy2hd/fonts/*" "copy2hd/setupart/MESSAGES.TXT" "text/*" -o"$data" >/dev/null
    # Every CD: backgrounds, panorama nodes, animation assets, sounds.
    for index in 1 2 3 4 5 6; do
        iso="$(find_iso "$index")" || continue
        7z x -y "$iso" "backs/*" "nodes/*" "anims/*" "sounds/*" -o"$data" >/dev/null 2>&1 || true
        extract_panorama_nodes "$iso"
        extract_ui_assets "$iso"
    done
    touch "$data/.media_v2"
    touch "$data/.ui_v1"
    touch "$data/.cursors_v1"
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
    for archive in CS CI CY CF CA CE CCB CCF CD; do
        target="$(lower "$archive")"
        mkdir -p "$data/$target"
        "$here/byzantine-all" "$data/copy2hd/engine/$archive.ALL" "$data/$target" >/dev/null
    done
    # The conversation text/clip archives are packed separately: CCB.ALL is the
    # bytecode, CCF.ALL the voice clip names and Byz.all the dialogue lines.
    7z x -y "$iso1" "copy2hd/engine/Byz.all" -o"$data" >/dev/null
    mkdir -p "$data/byz"
    "$here/byzantine-all" "$data/copy2hd/engine/Byz.all" "$data/byz" >/dev/null
    touch "$data/.prepared"
fi

# The top-bar screens need the shipped slot list and the movie/status files
# (0x41f065 reads autosave.btb + btb.sav, 0x4273f4 reads status.000, and YENİ
# OYUN loads new.can). Extract them once into an existing prepared dir.
if [ ! -f "$data/.menu_v1" ]; then
    7z x -y "$iso1" "copy2hd/savegame/BTB.SAV" "copy2hd/savegame/STATUS.000" \
        "copy2hd/savegame/NEW.CAN" "copy2hd/savegame/notebook.000" \
        "copy2hd/savegame/GAMEPREF.000" "text/*" \
        -o"$data" >/dev/null 2>&1 || true
    touch "$data/.menu_v1"
fi

# A data dir prepared by an older revision has no conversation archives; fetch
# just those instead of re-extracting everything.
if [ ! -d "$data/ccb" ] || [ ! -d "$data/ccf" ] || [ ! -d "$data/byz" ] || \
   [ ! -d "$data/cd" ]; then
    7z x -y "$iso1" "copy2hd/engine/CCB.ALL" "copy2hd/engine/CCF.ALL" \
        "copy2hd/engine/CD.ALL" "copy2hd/engine/Byz.all" -o"$data" >/dev/null
    for archive in CCB CCF CD; do
        target="$(lower "$archive")"
        mkdir -p "$data/$target"
        "$here/byzantine-all" "$data/copy2hd/engine/$archive.ALL" "$data/$target" >/dev/null
    done
    mkdir -p "$data/byz"
    "$here/byzantine-all" "$data/copy2hd/engine/Byz.all" "$data/byz" >/dev/null
fi

# An older data dir can have .prepared while missing some map destinations'
# media (the scene then composes black). Backfill every backs/nodes/anims/sounds
# tree once and mark it, so selecting a place from MAP has its assets.
if [ ! -f "$data/.media_v2" ]; then
    for index in 1 2 3 4 5 6; do
        iso="$(find_iso "$index")" || continue
        7z x -y "$iso" "backs/*" "nodes/*" "anims/*" "sounds/*" -o"$data" >/dev/null 2>&1 || true
        extract_panorama_nodes "$iso"
    done
    touch "$data/.media_v2"
fi

# An older prepared dir may lack the inventory/close-up assets; fetch them once.
if [ ! -f "$data/.ui_v1" ]; then
    for index in 1 2 3 4 5 6; do
        iso="$(find_iso "$index")" || continue
        extract_ui_assets "$iso"
    done
    touch "$data/.ui_v1"
fi

# ... or the cursor art (0x40dae5), which also lives under CD1's copy2hd.
if [ ! -f "$data/.cursors_v1" ]; then
    extract_ui_assets "$iso1"
    touch "$data/.cursors_v1"
fi

# A data dir prepared before the text step has neither the fonts nor the split
# CE records; fetch just those instead of re-extracting everything.
if [ ! -d "$data/copy2hd/fonts" ] || [ ! -d "$data/ce" ]; then
    7z x -y "$iso1" "copy2hd/fonts/*" "copy2hd/engine/CE.ALL" -o"$data" >/dev/null
    mkdir -p "$data/ce"
    "$here/byzantine-all" "$data/copy2hd/engine/CE.ALL" "$data/ce" >/dev/null
fi

# 0x41d45a reads setupart/MESSAGES.TXT for the common dialogs; the conversation
# exit dialog uses line 9 and labels 0x4c..0x4f.
if [ ! -f "$data/copy2hd/setupart/MESSAGES.TXT" ]; then
    7z x -y "$iso1" "copy2hd/setupart/MESSAGES.TXT" -o"$data" >/dev/null
fi

upper_scene="$(printf '%s' "$scene" | tr '[:lower:]' '[:upper:]')"
cc="$here/output/cc_all/$upper_scene"
cb="$here/output/cb_all/$upper_scene"
co="$here/output/co_all/$upper_scene"
[ -f "$cc" ] || { echo "no scene script for '$scene' ($cc)" >&2; exit 1; }

ce="$data/ce/$upper_scene"
[ -f "$ce" ] || ce=""
fonts="$data/copy2hd/fonts"
[ -d "$fonts" ] || fonts=""

# 0x40d2a1 plays dialog/<conversation>/<clip>.avi. Extract only the
# conversation folders of this scene (the scene's cd records), not every CD.
cd_records="$data/cd/$upper_scene"
if [ -f "$cd_records" ]; then
    names="$(LC_ALL=C tr '\0' '\n' <"$cd_records" | LC_ALL=C sed $'s/\xff//g' | \
        awk 'NF && (++n % 2 == 1)')"
    for name in $names; do
        folder="$(lower "$name")"
        [ -d "$data/dialog/$folder" ] && continue
        for index in 1 2 3 4 5 6; do
            iso="$(find_iso "$index")" || continue
            7z x -y "$iso" "dialog/$folder/*" -o"$data" >/dev/null 2>&1 || true
        done
    done
fi

save_path="$data/copy2hd/savegame/$save"
[ -f "$save_path" ] || { echo "no savegame '$save' in $data/copy2hd/savegame" >&2; exit 1; }

# The same animation name can contain DIFFERENT art on different CDs. The
# union extracted above is only a fallback, never the authority for collisions.
saved_disc="$("$here/byzantine-cc" --save-dump "$save_path" \
    --lcc "$data/copy2hd/engine/SHOW.LCC" | sed -n 's/^cd=\([1-6]\) .*/\1/p')"
[ -n "$saved_disc" ] || { echo "cannot determine saved media disc" >&2; exit 1; }
disc_iso="$(find_iso "$saved_disc")" || { echo "missing saved disc $saved_disc" >&2; exit 1; }
anims_overlay="$(python3 "$here/prepare_disc_anims.py" "$data" "$disc_iso" "$saved_disc")"

text_args=()
[ -d "$data/cursors" ] && text_args+=(--cursors "$data/cursors")
[ -n "$ce" ] && text_args+=(--ce "$ce")
[ -n "$fonts" ] && text_args+=(--fonts "$fonts")
messages="$data/copy2hd/setupart/MESSAGES.TXT"
[ -f "$messages" ] && text_args+=(--messages "$messages")

# A later 0x73 can reach a conversation, and a 0xbd can name a CF clip, outside
# the starting scene. The player extracts just that missing file into this
# analysis copy on demand. The saved disc goes first: the same clip name can
# carry different bytes on different CDs (the animation overlay above makes the
# same call), so the disc the save was made on is the authority.
media_isos="$data/media_isos.txt"
: >"$media_isos"
add_media_iso() {
    local iso iso_dir line
    iso="$(find_iso "$1")" || return 0
    iso_dir="$(cd "$(dirname "$iso")" && pwd)"
    line="$iso_dir/$(basename "$iso")"
    if grep -qxF "$line" "$media_isos"; then
        return 0
    fi
    printf '%s\n' "$line" >>"$media_isos"
}
add_media_iso "$saved_disc"
for index in 1 2 3 4 5 6; do
    add_media_iso "$index"
done

# 0x41f753(0) reads savegame/gamepref_000 (written by 0x41f7be when the
# preferences screen changes a value); the CD's GAMEPREF.000 is the default.
gamepref="$data/copy2hd/savegame/gamepref_000"
[ -f "$gamepref" ] || gamepref="$data/copy2hd/savegame/GAMEPREF.000"
[ -f "$gamepref" ] || gamepref="$data/copy2hd/goldsave/GAMEPREF.000"

config_arg=()
if [ -n "${BYZ_CONFIG:-}" ]; then
    config_arg+=(--config "$BYZ_CONFIG")
elif [[ " $* " != *" --config "* && " $* " != *" --no-config "* ]]; then
    config_arg+=(--config "$here/byzantine.ini")
fi

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
    --cd "$data/cd/$upper_scene" \
    --ccb "$data/ccb" \
    --ccf "$data/ccf" \
    --byz "$data/byz" \
    --dialog "$data/dialog" \
    --dialog-cache "$data/dialog_cache" \
    --media-isos "$media_isos" \
    --anims "$data" \
    --anims-overlay "$anims_overlay" \
    --sounds "$data" \
    --backgrounds "$data" \
    --gameprefs "$gamepref" \
    --inventor "$data/inventor.txt" \
    --hotitems "$data/hotitems.txt" \
    --languages "$data/lang" \
    "${config_arg[@]}" \
    "${text_args[@]}" \
    "${@:5}"
