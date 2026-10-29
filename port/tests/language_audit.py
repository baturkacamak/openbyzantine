#!/usr/bin/env python3
"""Black-box test for port/language_audit.py."""

from __future__ import annotations

import json
from pathlib import Path
import shutil
import subprocess
import sys
import tempfile

PORT = Path(__file__).resolve().parent.parent


def write(path: Path, data: str) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_bytes(data.encode("latin-1"))


def main() -> int:
    root = Path(tempfile.mkdtemp(prefix="byz-language-audit-test-"))
    try:
        base = root / "base"
        en = root / "en"
        write(base / "byz" / "A", "a\nb\nc\n")
        write(en / "byz" / "A", "a\nb\n")
        write(base / "ccf" / "A", "c1\nc2\n")
        write(en / "ccf" / "A", "c1\n")
        write(base / "ce" / "SC", "e1\n$e2\n")
        write(en / "ce" / "SC", "e1\n")
        write(base / "setupart" / "MESSAGES.TXT", "base\n")
        write(en / "setupart" / "MESSAGES.TXT",
              "\n".join(["line"] * 62 + ["Language: English", ""]))
        write(base / "backs" / "ma" / "X.jpg", "base art\n")
        write(en / "backs" / "ma" / "X.jpg", "en art\n")
        write(en / "dialog" / "x" / "a.avi", "voice\n")

        result = subprocess.run(
            [sys.executable, str(PORT / "language_audit.py"),
             "--base", str(base), "--pack", f"en={en}", "--json"],
            check=True, capture_output=True, text=True)
        reports = json.loads(result.stdout)
        assert len(reports) == 1, reports
        report = reports[0]
        assert report["english_font_branch"] is True, report
        assert report["different"] == 6, report
        assert report["byz_line_mismatches"], report
        assert report["ccf_line_mismatches"], report
        assert report["ce_entry_mismatches"], report
        assert report["by_category"]["dialog"]["added"] == 1, report
        assert report["by_category"]["backs"]["changed"] == 1, report
        assert not (base / "lang").exists(), "audit wrote into the base"

        print("language audit tests passed")
        return 0
    finally:
        shutil.rmtree(root, ignore_errors=True)


if __name__ == "__main__":
    raise SystemExit(main())
