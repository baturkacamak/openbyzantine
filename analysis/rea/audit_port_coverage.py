#!/usr/bin/env python3
"""Mechanical port-coverage audit.

Counts how many of the 840 game functions (0x401000-0x43ffff) are mentioned by
address anywhere under `port/` (code, headers, docs). That is a *documented
counterpart* count, not runtime equivalence: an address in a comment has a
documented behaviour, nothing more. Also splits the remaining functions into
those that call GDI/DirectDraw/DirectSound/AVI/mmio/dialog APIs and the rest.
"""

from __future__ import annotations

import json
import pathlib
import re

REA = pathlib.Path(__file__).resolve().parent
ROOT = REA.parent.parent
GAME_LOW, GAME_HIGH = 0x401000, 0x43ffff
API = re.compile(
    r"(BitBlt|StretchBlt|CreateDIBSection|DirectDraw|DirectSound|AVIStream\w*|"
    r"AVIFile\w*|ICOpen|ICLocate|DrawDib\w*|mmio\w*|timeSetEvent|GetDC|"
    r"TextOut\w*|CreateFont\w*|SelectPalette|SetPaletteEntries|"
    r"GetDIBColorTable|SetDIBColorTable|MessageBoxA|DialogBoxParamA|"
    r"CreateWindowExA|SetTimer)"
)


def main() -> int:
    index = json.loads((REA / "reconstruction/index.json").read_text())
    game = {
        f["address"]: f
        for f in index["functions"]
        if GAME_LOW <= int(f["address"], 16) <= GAME_HIGH
    }
    text = ""
    for path in (ROOT / "port").rglob("*"):
        if path.is_file() and path.suffix in (".cpp", ".hpp", ".md", ".py"):
            text += path.read_text(errors="replace")
    mentioned = {a.lower() for a in re.findall(r"0x4[0-3][0-9a-fA-F]{4}", text)}

    matched = [f for a, f in game.items() if a in mentioned]
    total_bytes = sum(f["size"] for f in game.values())
    matched_bytes = sum(f["size"] for f in matched)

    media: list[dict] = []
    other: list[dict] = []
    for address, function in game.items():
        if address in mentioned:
            continue
        body_path = REA / "pseudocode/functions" / f"{address}.c"
        body = body_path.read_text(errors="replace") if body_path.exists() else ""
        (media if API.search(body) else other).append(function)

    print(
        f"game functions {len(game)} ({total_bytes} B); documented counterparts "
        f"{len(matched)} ({100 * len(matched) / len(game):.1f}%), "
        f"{matched_bytes} B ({100 * matched_bytes / total_bytes:.1f}%)"
    )
    print(
        f"remaining {len(game) - len(matched)}: media/gui-api {len(media)} "
        f"({sum(f['size'] for f in media)} B), other {len(other)} "
        f"({sum(f['size'] for f in other)} B)"
    )
    print("largest remaining (other):")
    for function in sorted(other, key=lambda f: -f["size"])[:10]:
        print(f"  {function['address']} {function['size']} B")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
