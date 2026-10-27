#!/usr/bin/env python3
"""MR arrival: the entry 0x70 conversation restores the hotspots and the exit.

The reported bug: travelling to MR from MAP (the documented "click 47 twice"
route) left the scene with `hotspots=0` because the port skipped the arrival
dialogue (MMhmr1a) and therefore its continuation, which opens the door and
enables the view's hotspots. This drives the real scene.sh chain headlessly.
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
    with tempfile.TemporaryDirectory(prefix="mr-arrival-") as directory:
        base = [str(root / "port/scene.sh"), str(root), "MAP", "87", "27.btb",
                "--no-audio", "--no-video", "--click-at", "192,300",
                "--click-at", "192,300", "--conversation-choices", "0,0,0"]
        shots = {"arrival": [], "exit": ["589,349"]}
        for name, clicks in shots.items():
            args = list(base)
            for point in clicks:
                args += ["--click-at", point]
            args += ["--shot", str(Path(directory) / (name + ".ppm"))]
            result = subprocess.run(args, cwd=root, env=env, text=True,
                                    capture_output=True, timeout=180)
            log = result.stdout
            assert result.returncode == 0, log + result.stderr
            assert "entry_conversation record=0 instruction=169" in log, log
            assert "conversation record=0 name=MMhmr1a result=65535" in log, log
            assert "entry_no_hotspots" not in log, log
            assert "entry scene=MR steps=" in log, log
            mr = log.split("entry scene=MR ", 1)[1].splitlines()[0]
            assert "hotspots=2" in mr and "blocker=" not in mr, mr
            if name == "arrival":
                assert "composed background=MRDOOROP.jpg" in log, log
                assert "scene=MR background=MRDoorOp hotspots=2 slots=42,43," in log, log
                assert log.strip().splitlines()[-1].endswith("slots=42,43,"), log
            else:
                assert "click_at 589,349 -> 589,369 slot 43 action 160" in log, log
                assert "travel index=25 scene=map from=MR" in log, log
                assert log.strip().splitlines()[-1].endswith(
                    "slots=34,36,39,41,47,49,"), log
            print("MR arrival:", name, "PASS")


if __name__ == "__main__":
    main()
