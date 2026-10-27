#!/usr/bin/env python3
"""Real asset regression: actors and cross-scene conversation rebinding.

The initial --run-label 1790 is a diagnostic shortcut into MAP, not evidence
of a natural exit from EA. Later transitions use actual MAP/TK hotspots.
Video/audio decoding is checked separately; this test uses dummy devices.
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
    with tempfile.TemporaryDirectory(prefix="tk-media-") as output:
        command = [str(root / "port/scene.sh"), str(root), "EA", "602", "26.btb",
                   "--no-audio", "--no-video", "--run-label", "1790", "--click", "34",
                   "--click-at", "440,245", "--click-at", "390,269"]
        result = subprocess.run(command + ["--shot", str(Path(output) / "actor.ppm")],
                                cwd=root, env=env, text=True, capture_output=True, timeout=60)
        log = result.stdout + result.stderr
        assert result.returncode == 0, log
        assert "travel index=33 scene=tk from=MAP" in log, log
        assert "pixels=37500 drawn_ids=1" in log, log
        assert result.stdout.strip().splitlines()[-1].endswith("slots=3,4,5,"), log
        command += ["--click-at", "329,274", "--conversation-choices", "0,0,0,0",
                    "--shot", str(Path(output) / "done.ppm")]
        result = subprocess.run(command, cwd=root, env=env, text=True,
                                capture_output=True, timeout=60)
        log = result.stdout + result.stderr
        assert result.returncode == 0, log
        assert "conversation record=0 name=Inutk1a" in log, log
        assert "conversation_end record=0 name=Inutk1a" in log, log
        assert "clip=Nt1nur02" in log and "answer_lists=1" in log, log
        for failure in ["VOemphn2", "unsupported conversation", "missing (", "click_not_committed"]:
            assert failure not in log, log
        assert result.stdout.strip().splitlines()[-1].endswith("slots=4,5,"), log
        print("TK actor, EA -> MAP -> TK conversation rebinding and completion: PASS")


if __name__ == "__main__":
    main()
