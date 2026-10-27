#!/usr/bin/env python3
"""Real MCaEA3b dialogue and post-conversation exit.

Label 1599 is an explicit diagnostic shortcut to the failing 0x70; this is
not a full natural drawer traversal or a physical speaker check.
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
    with tempfile.TemporaryDirectory(prefix="ea-inline-voice-") as directory:
        command = [str(root / "port/scene.sh"), str(root), "EA", "602", "26.btb",
                   "--no-audio", "--no-video", "--run-label", "1599"]
        for name, clicks in [("conversation", []), ("exit", ["589,349"])]:
            args = list(command)
            for point in clicks:
                args += ["--click-at", point]
            args += ["--shot", str(Path(directory) / (name + ".ppm"))]
            result = subprocess.run(args, cwd=root, env=env, text=True,
                                    capture_output=True, timeout=60)
            log = result.stdout
            assert result.returncode == 0, log + result.stderr
            for error in ["unsupported conversation", "click_not_committed", "error="]:
                assert error not in log, log
            assert "conversation_end record=2 name=MCaEA3b result=65535 lines=3" in log, log
            assert log.index("index=2 clip=Mctthu00") < log.index("index=3 clip=Mctcar02"), log
            assert "conversation_variable local=0 before=0 after=1" in log, log
            assert "conversation_resume_probe steps=6" in log, log
            if name == "conversation":
                assert "composed background=ealookin.jpg" in log.lower(), log
                assert "159,160," in log.strip().splitlines()[-1], log
            else:
                assert "slot 160 action 1611" in log and "composed panorama=" in log, log
                assert log.strip().splitlines()[-1].endswith("slots=157,158,"), log
            print("EA inline VOICE:", name, "PASS")


if __name__ == "__main__":
    main()
