#!/usr/bin/env python3
"""Real EA panorama -> flat close-up -> exit via coordinate hit testing."""
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
    with tempfile.TemporaryDirectory(prefix="ea-layer-") as directory:
        command = [str(root / "port/scene.sh"), str(root), "EA", "602", "26.btb",
                   "--no-audio", "--click", "3"]
        for name, clicks in [("closeup", []), ("return", ["589,349"])]:
            args = list(command)
            for point in clicks:
                args += ["--click-at", point]
            args += ["--shot", str(Path(directory) / (name + ".ppm"))]
            result = subprocess.run(args, cwd=root, env=env, capture_output=True,
                                    text=True, timeout=60)
            log = result.stdout
            assert result.returncode == 0, log + result.stderr
            assert "click slot 3 action 544 handled=true" in log, log
            if not clicks:
                assert "composed background=eapupall.jpg" in log.lower(), log
                assert log.strip().splitlines()[-1].endswith("slots=18,19,"), log
            else:
                assert "click_at 589,349 -> 589,369 slot 18 action 551" in log, log
                assert "hit no hotspot" not in log, log
                assert "composed panorama=" in log, log
                assert log.strip().splitlines()[-1].endswith("slots=1,2,3,4,5,6,7,"), log
            print("EA panorama/close-up coordinate routing:", name, "PASS")


if __name__ == "__main__":
    main()
