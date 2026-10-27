#!/usr/bin/env python3
"""Exercise the real player close-up route using extracted, user-owned data.

Usage: python3 analysis/rea/verify_examine_controls.py /tmp/byz-conv
The normal make tests cover the selection/gate contract; this optional test
checks actual EXAMINE scripts, item switching and parent return together.
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
    with tempfile.TemporaryDirectory(prefix="byz-examine-controls-") as output:
        command = [str(root / "port/scene.sh"), str(root), "EA", "602", "26.btb",
                   "--no-audio"]
        baseline = subprocess.run(
            command + ["--shot", str(Path(output) / "arrival.ppm")],
            cwd=root, env=env, text=True, capture_output=True, timeout=60)
        assert baseline.returncode == 0, baseline.stdout + baseline.stderr
        arrival_slots = baseline.stdout.strip().splitlines()[-1].split("slots=")[1]
        command += ["--bar-select", "17", "--bar-use"]
        # Camera action, switch to the note, switch back, repeat action, exit.
        for point in ["210,280", "190,450", "310,450", "210,280", "589,349"]:
            command += ["--click-at", point]
        command += ["--shot", str(Path(output) / "returned.ppm")]
        result = subprocess.run(command, cwd=root, env=env, text=True,
                                capture_output=True, timeout=60)
        if result.returncode:
            raise RuntimeError(result.stdout + result.stderr)
        log = result.stdout
        assert log.count("slot 91 action 288 click_result steps=3") == 2, log
        assert "item_use rejected" not in log, log
        assert "closeup_switch scene=EXAMINE item=30" in log, log
        assert "closeup_switch scene=EXAMINE item=17" in log, log
        assert "closeup_return scene=EA from=EXAMINE" in log, log
        assert log.strip().splitlines()[-1].endswith("slots=" + arrival_slots), log
        assert "bar=visible" in log, log
        assert "examined=yes" in log, log
        print("Real camera action twice, item switch and EA parent return: PASS")


if __name__ == "__main__":
    main()
