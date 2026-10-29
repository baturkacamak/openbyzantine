#!/usr/bin/env python3
"""Black-box test for port/language_pack.py (no game data needed)."""

from __future__ import annotations

import json
from pathlib import Path
import shutil
import subprocess
import sys
import tempfile

PORT = Path(__file__).resolve().parent.parent


def write(path: Path, data: bytes | str) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    if isinstance(data, str):
        data = data.encode("latin-1")
    path.write_bytes(data)


def main() -> int:
    root = Path(tempfile.mkdtemp(prefix="byz-language-pack-test-"))
    try:
        base = root / "base"
        source = root / "en"
        write(base / "byz" / "A", "base text\n")
        write(base / "byz" / "SAME", "same text\n")
        write(base / "fonts" / "KLIO.SFN", "font\n")
        write(base / "setupart" / "MESSAGES.TXT", "base\n")
        write(base / "backs" / "ma" / "X.jpg", "base art\n")
        write(source / "byz" / "A", "english text\n")
        write(source / "byz" / "SAME", "same text\n")
        write(source / "fonts" / "KLIO.SFN", "font\n")
        lines = [b"line\n"] * 62 + [b"Language: English\n"]
        write(source / "setupart" / "MESSAGES.TXT", b"".join(lines))
        write(source / "backs" / "ma" / "X.jpg", "english art\n")
        write(source / "dialog" / "x" / "a.avi", "voice\n")
        write(source / "sounds" / "v.wav", "voice\n")

        command = [
            sys.executable,
            str(PORT / "language_pack.py"),
            "--base", str(base),
            "--source", str(source),
            "--code", "en",
            "--name", "English",
            "--json",
        ]
        result = subprocess.run(command, check=True, capture_output=True, text=True)
        report = json.loads(result.stdout)
        assert report["english_font_branch"] is True, report
        assert report["counts"]["changed"] == 3, report
        assert report["counts"]["added"] == 2, report
        assert report["counts"]["same"] == 2, report

        pack = base / "lang" / "en"
        assert (pack / "byz" / "A").read_text() == "english text\n"
        assert not (pack / "byz" / "SAME").exists()
        assert not (pack / "fonts" / "KLIO.SFN").exists()
        assert (pack / "setupart" / "MESSAGES.TXT").exists()
        assert (pack / "backs" / "ma" / "X.jpg").read_text() == "english art\n"
        assert (pack / "dialog" / "x" / "a.avi").exists()
        assert (pack / "sounds" / "v.wav").exists()
        manifest = (pack / "pack.ini").read_text()
        assert "code=en" in manifest and "text=1" in manifest and "voice=1" in manifest
        assert "font_branch=english" in manifest
        saved = json.loads((pack / "build-report.json").read_text())
        assert saved["counts"]["changed"] == 3

        # Text-only must not copy voice files, and dry-run must not write.
        shutil.rmtree(pack)
        subprocess.run(command[:-1] + ["--text-only", "--json"], check=True,
                       capture_output=True, text=True)
        assert (pack / "byz" / "A").exists()
        assert not (pack / "dialog" / "x" / "a.avi").exists()
        assert "voice=0" in (pack / "pack.ini").read_text()
        shutil.rmtree(pack)
        subprocess.run(command[:-1] + ["--dry-run", "--json"], check=True,
                       capture_output=True, text=True)
        assert not pack.exists()

        print("language pack tests passed")
        return 0
    finally:
        shutil.rmtree(root, ignore_errors=True)


if __name__ == "__main__":
    raise SystemExit(main())
