#!/usr/bin/env python3
"""Real TK view-switch regression, using an existing extracted media root."""
import os
from pathlib import Path
import subprocess
import sys
import tempfile


def main():
    root = Path(__file__).resolve().parents[2]
    data = Path(sys.argv[1]).resolve()
    command = [str(root / "port/byzantine-play"),
               str(root / "port/output/cc_all/TK"),
               str(root / "port/output/cb_all/TK"), "--entry",
               "--engine", str(data / "copy2hd/engine"),
               "--backgrounds", str(data), "--anims", str(data), "--no-audio"]
    for option, path in [("ca", "ca/TK"), ("ci", "ci/TK"), ("cy", "cy/TK"),
                         ("cs", "cs/TK"), ("cf", "cf/TK"),
                         ("lci", "copy2hd/engine/SHOW.LCI"),
                         ("lcy", "copy2hd/engine/SHOW.LCY"),
                         ("lcx", "copy2hd/engine/SHOW.LCX")]:
        command += ["--" + option, str(data / path)]
    env = dict(os.environ, SDL_VIDEODRIVER="dummy", SDL_AUDIODRIVER="dummy")
    with tempfile.TemporaryDirectory(prefix="tk-hotspots-") as output:
        cases = [(["390,269"], "3,4,5,"),
                 (["390,269", "428,201"], "6,"),
                 (["390,269", "428,201", "589,349", "589,349"], "1,2,")]
        for index, (points, slots) in enumerate(cases):
            args = list(command)
            for point in points:
                args += ["--click-at", point]
            args += ["--shot", str(Path(output) / f"view-{index}.ppm")]
            result = subprocess.run(args, cwd=root, env=env, text=True,
                                    capture_output=True, timeout=30)
            assert result.returncode == 0, result.stdout + result.stderr
            final = result.stdout.strip().splitlines()[-1]
            assert final.endswith("slots=" + slots), result.stdout
            assert result.stdout.count("slot 2 action 61") == 1, result.stdout
            if len(points) > 1:
                assert "slot 5 action 87" in result.stdout, result.stdout
            print(f"TK {points}: active slots {slots} PASS")


if __name__ == "__main__":
    main()
