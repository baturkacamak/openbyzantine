#!/bin/sh
# Run one scene directly (no save) the way the coverage census does, so scenes
# without a shipped save can still be inspected.
set -e
data=${1:?data dir}
scene=${2:?scene}
shift 2
port=$(cd "$(dirname "$0")" && pwd)
set -- "$port/byzantine-play" "$port/output/cc_all/$scene" "$port/output/cb_all/$scene" 0 \
  --engine "$data/copy2hd/engine" \
  --lcx "$data/copy2hd/engine/SHOW.LCX" \
  --lci "$data/copy2hd/engine/SHOW.LCI" \
  --lcy "$data/copy2hd/engine/SHOW.LCY" \
  --ccb "$data/ccb" --ccf "$data/ccf" --byz "$data/byz" \
  --dialog "$data/dialog" --dialog-cache "$data/dialog_cache" \
  --anims "$data" --sounds "$data" --backgrounds "$data" \
  --gameprefs "$data/copy2hd/goldsave/GAMEPREF.000" \
  --inventor "$data/inventor.txt" --hotitems "$data/hotitems.txt" \
  --fonts "$data/copy2hd/fonts" \
  --messages "$data/copy2hd/setupart/MESSAGES.TXT" \
  --entry --no-audio --no-video --no-config "$@"
for table in ci cy cf cs ca cd ce; do
  if [ -f "$data/$table/$scene" ]; then
    set -- "$@" "--$table" "$data/$table/$scene"
  fi
done
if [ -f "$port/output/co_all/$scene" ]; then
  set -- "$@" --co "$port/output/co_all/$scene"
fi
SDL_VIDEODRIVER=dummy SDL_AUDIODRIVER=dummy exec "$@"
