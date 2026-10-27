#!/usr/bin/env python3
"""Extract an immutable per-disc animation overlay; never flatten CD variants."""
import hashlib
import json
from pathlib import Path
import shutil
import subprocess
import sys
import tempfile


def main():
    data, iso = map(lambda s: Path(s).resolve(), sys.argv[1:3])
    disc = int(sys.argv[3])
    if not 1 <= disc <= 6:
        raise ValueError("saved disc must be in 1..6")
    stat = iso.stat()
    identity = {"iso": str(iso), "size": stat.st_size, "mtime_ns": stat.st_mtime_ns,
                "disc": disc, "version": 1}
    key = hashlib.sha256(json.dumps(identity, sort_keys=True).encode()).hexdigest()[:16]
    parent = data / "disc-media"
    parent.mkdir(parents=True, exist_ok=True)
    target = parent / f"cd{disc}-{key}"
    if not (target / "ready.json").is_file():
        temporary = Path(tempfile.mkdtemp(prefix=f".cd{disc}-", dir=parent))
        try:
            print(f"animation_media extracting disc={disc} source={iso.name}", file=sys.stderr)
            result = subprocess.run(["7z", "x", "-y", str(iso), "anims/*", f"-o{temporary}"],
                                    capture_output=True, text=True)
            if result.returncode != 0 or not (temporary / "anims").is_dir():
                raise RuntimeError("disc animation extraction failed: " + result.stderr[-2000:])
            (temporary / "ready.json").write_text(json.dumps(identity, indent=2) + "\n")
            try:
                temporary.rename(target)
            except OSError:
                if not (target / "ready.json").is_file():
                    raise
        finally:
            if temporary.exists():
                shutil.rmtree(temporary)
    print(f"animation_media disc={disc} overlay={target}", file=sys.stderr)
    print(target)


if __name__ == "__main__":
    main()
