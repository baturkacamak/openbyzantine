#!/usr/bin/env python3
"""EA photo close-up keeps its desk background after the hotspot action.

Action 1442 (slot 68 on the EA HKey photo close-up) executes instructions
1810..1815: 0x6e/0x85/0x64/0x6e/0x6d/0x6f. It loads no 0x69 background, so the
original keeps the desk picture. The frame used to fall back to the scene's
first static 0x69 entry (EAEMShMs), which showed the shelf with the picture
frames instead of the photos the player was looking at.
"""
import os
from pathlib import Path
import subprocess
import sys
import tempfile


def main():
    root = Path(__file__).resolve().parents[2]
    data = Path(sys.argv[1]).resolve()
    env = dict(os.environ, BYZ_DATA=str(data), SDL_VIDEODRIVER="dummy",
               SDL_AUDIODRIVER="dummy")
    with tempfile.TemporaryDirectory(prefix="ea-photo-") as directory:
        shot = Path(directory) / "photo.ppm"
        command = [str(root / "port/scene.sh"), str(root), "EA", "602", "26.btb",
                   "--no-audio", "--no-video", "--print-hotspots", "--print-records",
                   "--run-label", "653",       # EA00 -> EA01 panorama
                   "--click", "28",            # desk close-up
                   "--click-at", "295,261",    # blotting pad
                   "--click-at", "314,248",    # photo close-up (EA HKey)
                   "--click-at", "354,236",    # photo hotspot, action 1442
                   "--shot", str(shot)]
        result = subprocess.run(command, cwd=root, env=env, capture_output=True,
                                text=True, timeout=120)
        log = result.stdout
        assert result.returncode == 0, log + result.stderr
        assert "click_at 314,248 -> 314,268 slot 64 action 1415" in log, log
        assert "composed background=EAHKEY.JPG" in log, log
        assert "composed background=EAEmShMs" not in log, log
        assert "click_at 354,236 -> 354,256 slot 68 action 1442" in log, log
        tail = log[log.index("click_at 354,236"):]
        assert "steps=6" in tail, log
        # The desk picture survives and the flipped polaroid (record 16) is
        # drawn on it: the action reveals the photo instead of changing scene.
        assert "composed background=EAHKEY.JPG" in tail, tail
        assert "drawn_ids=16" in tail, tail
        assert "record id=16" in tail and "asset=EATexflp" in tail, tail
        assert "scene=EA background=EAHKey" in tail, tail
        assert shot.exists() and shot.stat().st_size > 0, "no composed frame"
    print("EA photo close-up keeps its desk background: PASS")


if __name__ == "__main__":
    main()
