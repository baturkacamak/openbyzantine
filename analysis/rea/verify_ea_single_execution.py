#!/usr/bin/env python3
"""Real EA desk: a probe must not be used as the execution's input state.

Label 653 is a diagnostic shortcut to the room reached by the real slot-2
action. This verifies script state and exit, not the request-only homer1 video.
"""
import os
from pathlib import Path
import subprocess
import sys
import tempfile


def main():
    root = Path(__file__).resolve().parents[2]
    env = dict(os.environ, BYZ_DATA=str(Path(sys.argv[1]).resolve()),
               SDL_VIDEODRIVER="dummy", SDL_AUDIODRIVER="dummy")
    with tempfile.TemporaryDirectory(prefix="ea-single-execution-") as directory:
        command = [str(root / "port/scene.sh"), str(root), "EA", "602", "26.btb",
                   "--no-audio", "--run-label", "653", "--click", "28"]
        cases = [("desk", [], "36,38,39,41,"),
                 ("object", ["133,245"], "37,"),
                 ("exit", ["133,245", "589,349"], "27,28,29,30,31,32,33,34,35,")]
        for name, clicks, expected in cases:
            args = list(command)
            for point in clicks:
                args += ["--click-at", point]
            args += ["--print-variables", "--shot", str(Path(directory) / (name + ".ppm"))]
            result = subprocess.run(args, cwd=root, env=env, text=True,
                                    capture_output=True, timeout=60)
            log = result.stdout
            assert result.returncode == 0, log + result.stderr
            assert log.strip().splitlines()[-1].endswith("slots=" + expected), log
            if name == "desk":
                assert "variable local=43 value=0 " in log, log
            else:
                assert "slot 41 action 1377" in log, log
                assert "variable local=43 value=1 " in log, log
                assert "variable local=51 value=1 " in log, log
            if name == "object":
                assert "composed background=EASHOMER.JPG" in log, log
            if name == "exit":
                assert "slot 37 action 1392" in log and "composed panorama=" in log, log
            print("EA execution starts before probe:", name, "PASS")


if __name__ == "__main__":
    main()
