#!/usr/bin/env python3
"""Build a language overlay from a prepared localized data tree.

The port never needs a full second copy of the game. This tool compares a
prepared base tree with a prepared language tree and copies only the files that
actually differ into <base>/lang/<code>. That is the automatic part of the
localization audit: no per-image OCR or hand-written list is needed.

The source tree must already have the split tables scene.sh produces (ce/,
byz/, ccf/, ...; use scene.sh once with the localized discs, then pass its
BYZ_DATA directory here). The base is normally the Turkish prepared tree.

Example:
  ./port/language_pack.py --base /tmp/byz-scene --code en \
      --source /tmp/byz-scene-en --name English

The resulting pack is:
  /tmp/byz-scene/lang/en/pack.ini
  /tmp/byz-scene/lang/en/<only the differing files>

Text and voice can be selected independently at runtime:
  ./port/scene.sh . MAP 87 tempsave.btb --text-lang en --voice-lang tr
"""

from __future__ import annotations

import argparse
import hashlib
import json
import os
from pathlib import Path
import shutil
import sys

TEXT_DIRS = (
    "ce",
    "byz",
    "fonts",
    "setupart",
    "text",
    "inventor",
    "backs",
    "nodes",
)
VOICE_DIRS = (
    "ccf",
    "dialog",
    "video",
    "sounds",
    "avi",
)


def canonical(relative: str) -> str:
    """The shipped tree sometimes has copy2hd/ in front of the same file."""
    lowered = relative.replace("\\", "/")
    if lowered.startswith("copy2hd/"):
        return lowered[len("copy2hd/") :]
    return lowered


def selected(relative: str, want_text: bool, want_voice: bool) -> bool:
    parts = canonical(relative).split("/", 1)
    if len(parts) != 2:
        return False
    top = parts[0].lower()
    if want_text and top in TEXT_DIRS:
        return True
    if want_voice and top in VOICE_DIRS:
        return True
    return False


def digest(path: Path) -> str:
    hasher = hashlib.sha256()
    with path.open("rb") as handle:
        for chunk in iter(lambda: handle.read(1024 * 1024), b""):
            hasher.update(chunk)
    return hasher.hexdigest()


def inventory(root: Path) -> dict[str, Path]:
    """Relative lowercase path -> exact path. First exact spelling wins."""
    result: dict[str, Path] = {}
    for path in sorted(root.rglob("*")):
        if not path.is_file():
            continue
        relative = path.relative_to(root).as_posix()
        result.setdefault(canonical(relative).lower(), path)
    return result


def messages_line(root: Path, index: int) -> str | None:
    for relative in ("setupart/MESSAGES.TXT", "copy2hd/setupart/MESSAGES.TXT"):
        path = root / relative
        if not path.is_file():
            continue
        with path.open("rb") as handle:
            for at, line in enumerate(handle):
                if at == index:
                    return line.rstrip(b"\r\n").decode("latin-1")
    return None


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--base", required=True, type=Path,
                        help="prepared base data root (the flat scene.sh tree)")
    parser.add_argument("--source", required=True, type=Path,
                        help="prepared localized data root")
    parser.add_argument("--code", required=True,
                        help="short pack code, e.g. en/de/fr/es/pt/tr")
    parser.add_argument("--name", default="",
                        help="display name (defaults to the code)")
    parser.add_argument("--encoding", default="raw",
                        help="MESSAGES.TXT encoding: raw/cp1254 (default raw)")
    parser.add_argument("--text-only", action="store_true",
                        help="build only the text overlay")
    parser.add_argument("--voice-only", action="store_true",
                        help="build only the voice overlay")
    parser.add_argument("--dry-run", action="store_true",
                        help="report without writing")
    parser.add_argument("--clean", action="store_true",
                        help="remove an existing <base>/lang/<code> first")
    parser.add_argument("--json", action="store_true",
                        help="machine-readable report")
    return parser.parse_args()


def main() -> int:
    args = parse_args()
    base: Path = args.base.resolve()
    source: Path = args.source.resolve()
    code = args.code.strip().lower()
    if not code or "/" in code or "\\" in code:
        print("language_pack: invalid --code", file=sys.stderr)
        return 2
    if not base.is_dir() or not source.is_dir():
        print("language_pack: --base and --source must be directories", file=sys.stderr)
        return 2
    if args.text_only and args.voice_only:
        print("language_pack: choose --text-only or --voice-only, not both", file=sys.stderr)
        return 2
    want_text = not args.voice_only
    want_voice = not args.text_only

    target = base / "lang" / code
    if args.clean and target.exists():
        if args.dry_run:
            print("would remove", target)
        else:
            shutil.rmtree(target)

    base_inventory = inventory(base)
    source_inventory = inventory(source)
    base_hashes: dict[str, str] = {}
    counts = {"added": 0, "changed": 0, "same": 0, "bytes": 0, "skipped": 0}
    examples: list[tuple[str, str]] = []
    for key, path in sorted(source_inventory.items()):
        relative = path.relative_to(source).as_posix()
        if not selected(relative, want_text, want_voice):
            continue
        if key.startswith("lang/"):
            continue
        base_path = base_inventory.get(key)
        if base_path is not None:
            if key not in base_hashes:
                base_hashes[key] = digest(base_path)
            if base_hashes[key] == digest(path):
                counts["same"] += 1
                continue
            kind = "changed"
        else:
            kind = "added"
        counts[kind] += 1
        counts["bytes"] += path.stat().st_size
        if len(examples) < 12:
            examples.append((kind, relative))
        if not args.dry_run:
            destination = target / relative
            destination.parent.mkdir(parents=True, exist_ok=True)
            shutil.copy2(path, destination)

    marker = messages_line(source, 0x3E)
    english = bool(marker and "English" in marker)
    manifest = (
        "[pack]\n"
        f"code={code}\n"
        f"name={args.name or code}\n"
        f"text={1 if want_text else 0}\n"
        f"voice={1 if want_voice else 0}\n"
        f"messages_encoding={args.encoding}\n"
        f"font_branch={'english' if english else 'other'}\n"
    )
    if not args.dry_run:
        target.mkdir(parents=True, exist_ok=True)
        (target / "pack.ini").write_text(manifest, encoding="utf-8")
        # Keep the audit with the pack so a later run can see what was chosen.
        report = {
            "code": code,
            "name": args.name or code,
            "source": str(source),
            "text": want_text,
            "voice": want_voice,
            "messages_encoding": args.encoding,
            "language_marker": marker,
            "english_font_branch": english,
            "counts": counts,
            "examples": examples,
        }
        (target / "build-report.json").write_text(
            json.dumps(report, indent=2, ensure_ascii=False) + "\n", encoding="utf-8"
        )

    if args.json:
        print(json.dumps({
            "target": str(target),
            "counts": counts,
            "english_font_branch": english,
            "examples": examples,
        }, indent=2, ensure_ascii=False))
    else:
        print(f"language_pack code={code} target={target}")
        print(f"  text={int(want_text)} voice={int(want_voice)} "
              f"encoding={args.encoding} english_font={int(english)}")
        print(f"  changed={counts['changed']} added={counts['added']} "
              f"same={counts['same']} bytes={counts['bytes']}")
        for kind, relative in examples:
            print(f"  {kind}: {relative}")
        if args.dry_run:
            print("  dry-run: nothing written")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
