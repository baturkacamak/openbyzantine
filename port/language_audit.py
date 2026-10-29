#!/usr/bin/env python3
"""Audit prepared language trees against the prepared base tree.

This is the read-only step to run before building a pack. It does not need the
original executables and does not OCR images: it reports which files actually
differ, how large the overlays are, and whether the split dialogue tables keep
the same line/entry counts.

  ./port/language_audit.py --base /tmp/byz-scene \
      --pack en=/tmp/byz-scene-en --pack de=/tmp/byz-scene-de

The output is Markdown by default; --json is machine-readable. It is safe to
run before `language_pack.py`; nothing is written.
"""

from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path
import sys

TEXT = {"ce", "byz", "fonts", "setupart", "text", "inventor", "backs", "nodes"}
VOICE = {"ccf", "dialog", "video", "sounds", "avi"}


def canonical(relative: str) -> str:
    value = relative.replace("\\", "/")
    return value[8:] if value.startswith("copy2hd/") else value


def digest(path: Path) -> str:
    hasher = hashlib.sha256()
    with path.open("rb") as handle:
        for chunk in iter(lambda: handle.read(1024 * 1024), b""):
            hasher.update(chunk)
    return hasher.hexdigest()


def inventory(root: Path) -> dict[str, Path]:
    result: dict[str, Path] = {}
    for path in sorted(root.rglob("*")):
        if not path.is_file():
            continue
        relative = path.relative_to(root).as_posix()
        result.setdefault(canonical(relative).lower(), path)
    return result


def message_marker(root: Path) -> str | None:
    for relative in ("setupart/MESSAGES.TXT", "copy2hd/setupart/MESSAGES.TXT"):
        path = root / relative
        if not path.is_file():
            continue
        with path.open("rb") as handle:
            for index, line in enumerate(handle):
                if index == 0x3E:
                    return line.rstrip(b"\r\n").decode("latin-1")
    return None


def table_lines(path: Path) -> int:
    try:
        data = path.read_bytes()
    except OSError:
        return 0
    return data.count(b"\n") + (1 if data and not data.endswith(b"\n") else 0)


def ce_entries(path: Path) -> int:
    try:
        data = path.read_bytes()
    except OSError:
        return 0
    # 0x412206: a line starting with '$' closes one entry.
    count = 0
    for line in data.splitlines():
        if line.startswith(b"$"):
            count += 1
    return count


def category(relative: str) -> str:
    return relative.split("/", 1)[0].lower()


def audit_pack(base: Path, base_inventory: dict[str, Path], code: str,
               root: Path) -> dict:
    pack_inventory = inventory(root)
    differences: list[tuple[str, str, int]] = []
    missing: list[str] = []
    for key, path in pack_inventory.items():
        if key.startswith("lang/"):
            continue
        base_path = base_inventory.get(key)
        relative = path.relative_to(root).as_posix()
        if base_path is None:
            differences.append(("added", relative, path.stat().st_size))
            continue
        if digest(base_path) != digest(path):
            differences.append(("changed", relative, path.stat().st_size))
    for key in base_inventory:
        if key.startswith("lang/"):
            continue
        if key not in pack_inventory:
            missing.append(key)

    by_category: dict[str, dict[str, int]] = {}
    for kind, relative, size in differences:
        top = category(relative)
        entry = by_category.setdefault(top, {"changed": 0, "added": 0, "bytes": 0})
        entry[kind] += 1
        entry["bytes"] += size

    byz_mismatch = []
    for key, path in pack_inventory.items():
        if category(key) != "byz":
            continue
        base_path = base_inventory.get(key)
        if base_path is None:
            continue
        left = table_lines(path)
        right = table_lines(base_path)
        if left != right:
            byz_mismatch.append({"name": key, "pack": left, "base": right})
    ccf_mismatch = []
    for key, path in pack_inventory.items():
        if category(key) != "ccf":
            continue
        base_path = base_inventory.get(key)
        if base_path is None:
            continue
        left = table_lines(path)
        right = table_lines(base_path)
        if left != right:
            ccf_mismatch.append({"name": key, "pack": left, "base": right})
    ce_mismatch = []
    for key, path in pack_inventory.items():
        if category(key) != "ce":
            continue
        base_path = base_inventory.get(key)
        if base_path is None:
            continue
        left = ce_entries(path)
        right = ce_entries(base_path)
        if left != right:
            ce_mismatch.append({"name": key, "pack": left, "base": right})

    marker = message_marker(root)
    return {
        "code": code,
        "root": str(root),
        "language_marker": marker,
        "english_font_branch": bool(marker and "English" in marker),
        "different": len(differences),
        "bytes": sum(item[2] for item in differences),
        "by_category": by_category,
        "missing_files": len(missing),
        "missing_examples": missing[:20],
        "byz_line_mismatches": byz_mismatch,
        "ccf_line_mismatches": ccf_mismatch,
        "ce_entry_mismatches": ce_mismatch,
        "examples": [f"{kind}: {relative}" for kind, relative, _ in differences[:20]],
    }


def parse_pack(value: str) -> tuple[str, Path]:
    if "=" not in value:
        raise argparse.ArgumentTypeError("--pack expects code=path")
    code, path = value.split("=", 1)
    return code.strip().lower(), Path(path)


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--base", required=True, type=Path)
    parser.add_argument("--pack", action="append", type=parse_pack, default=[],
                        help="code=prepared-language-root (repeatable)")
    parser.add_argument("--json", action="store_true")
    args = parser.parse_args()
    base = args.base.resolve()
    if not base.is_dir():
        print("language_audit: --base is not a directory", file=sys.stderr)
        return 2
    base_inventory = inventory(base)
    reports = []
    for code, root in args.pack:
        root = root.resolve()
        if not root.is_dir():
            print(f"language_audit: missing pack root {root}", file=sys.stderr)
            continue
        reports.append(audit_pack(base, base_inventory, code, root))
    if args.json:
        print(json.dumps(reports, indent=2, ensure_ascii=False))
    else:
        print(f"# Language pack audit\n\nBase: `{base}`\n")
        for report in reports:
            print(f"## `{report['code']}`\n")
            print(f"- root: `{report['root']}`")
            print(f"- language marker: `{report['language_marker']}`")
            print(f"- English font branch: {report['english_font_branch']}")
            print(f"- differing files: {report['different']} "
                  f"({report['bytes']} bytes)")
            for name, counts in sorted(report["by_category"].items()):
                print(f"- `{name}`: changed={counts['changed']} "
                      f"added={counts['added']} bytes={counts['bytes']}")
            if report["missing_files"]:
                print(f"- missing from pack: {report['missing_files']} "
                      f"(examples: {', '.join(report['missing_examples'][:5])})")
            for label, key in (("BYZ line", "byz_line_mismatches"),
                               ("CCF line", "ccf_line_mismatches"),
                               ("CE entry", "ce_entry_mismatches")):
                if report[key]:
                    print(f"- {label} mismatches: {len(report[key])} "
                          f"(first: {report[key][0]})")
            if report["examples"]:
                print("- examples: " + "; ".join(report["examples"][:8]))
            print()
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
