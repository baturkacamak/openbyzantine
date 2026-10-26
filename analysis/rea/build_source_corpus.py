#!/usr/bin/env python3
"""Build a navigable source corpus from saved REA/Ghidra decompile exports.

Inputs (read-only):
  pseudocode/decompile-index.json   function inventory, ranges and hashes
  pseudocode/functions/<addr>.c     saved provider pseudocode

Outputs (analysis/rea/reconstruction/):
  README.md          what this corpus is and is not
  index.md           complete function inventory
  index.json         machine-readable inventory
  symbols.csv        address,name,bytes,ranges,coverage
  graph.json         call edges, callers/callees
  modules.md         regional/utility/call-graph overview
  functions/<addr>.c normalized pseudocode with a compact header

Every written export hash is re-checked against the index, so a stale or
edited file fails the build instead of entering the corpus.

This is decompiler output, not original source. It is not compilable and does
not recover original identifiers, types or comments.
"""

from __future__ import annotations

import hashlib
import json
import re
import sys
from collections import Counter, defaultdict
from pathlib import Path

REA = Path(__file__).resolve().parent
PSEUDOCODE = REA / "pseudocode"
INDEX = PSEUDOCODE / "decompile-index.json"
OUT = REA / "reconstruction"

FUN_RE = re.compile(r"\bFUN_([0-9a-fA-F]{8})\b")
DAT_RE = re.compile(r"\b(?:_?DAT|PTR|s)_([0-9a-fA-F]{6,8})\b")
STRING_RE = re.compile(r'"((?:[^"\\]|\\.){2,})"')
RANGE_RE = re.compile(r"^// range: (0x[0-9a-f]+)-(0x[0-9a-f]+) inclusive$", re.M)
COVERAGE_ROW = re.compile(
    r"^\|\s*(0x[0-9a-f]{6})\s*\|\s*([^|]+?)\s*\|\s*(bounded|partial)\s*\|$",
    re.M,
)


def load_coverage() -> dict[str, tuple[str, str]]:
    """Address -> (ported behavior, scope) from the manual audit table."""
    path = REA / "IMPLEMENTATION_QUICK_AUDIT.md"
    if not path.exists():
        return {}
    text = path.read_text(encoding="utf-8", errors="replace")
    return {
        addr.lower(): (behavior.strip(), scope)
        for addr, behavior, scope in COVERAGE_ROW.findall(text)
    }


def normalize_ascii(text: str) -> str:
    return text.encode("ascii", "replace").decode("ascii")


def split_body(raw: str) -> tuple[list[str], str]:
    """Return (header lines, pseudocode body)."""
    lines = raw.splitlines()
    body_start = 0
    for i, line in enumerate(lines):
        if line.startswith("// pseudocode:"):
            body_start = i + 1
            break
    else:
        body_start = len(lines)
    return lines[:body_start], "\n".join(lines[body_start:]).strip("\n")


def main() -> int:
    index = json.loads(INDEX.read_text(encoding="utf-8"))
    target = index.get("target_sha256")
    profile = index.get("analysis_profile_digest")
    entries = index["functions"]

    written = [e for e in entries if e.get("status") == "written"]
    external = [e for e in entries if e.get("status") != "written"]
    coverage = load_coverage()

    funcs: dict[str, dict] = {}
    failures: list[str] = []
    hash_checked = 0

    for entry in written:
        addr = entry["address"].lower()
        path = PSEUDOCODE / entry["relative_path"]
        raw_bytes = path.read_bytes()
        digest = hashlib.sha256(raw_bytes).hexdigest()
        if digest != entry["sha256"]:
            failures.append(f"{addr}: hash mismatch ({digest})")
            continue
        hash_checked += 1

        raw = raw_bytes.decode("utf-8", errors="replace")
        header, body = split_body(raw)
        ranges = [
            (int(start, 16), int(end, 16))
            for start, end in RANGE_RE.findall("\n".join(header))
        ]
        size = sum(end - start + 1 for start, end in ranges)
        callees = sorted(
            {"0x%x" % int(m.group(1), 16) for m in FUN_RE.finditer(body)}
        )
        data_refs = sorted(
            {"0x%x" % int(m.group(1), 16) for m in DAT_RE.finditer(body)}
        )
        strings = sorted({s for s in STRING_RE.findall(body) if len(s) >= 3})
        funcs[addr] = {
            "address": addr,
            "name": entry["name"],
            "size": size,
            "ranges": [[hex(s), hex(e)] for s, e in ranges],
            "non_contiguous": entry["body"].get("non_contiguous", False),
            "callees": callees,
            "data_refs": data_refs,
            "strings": strings,
            "body": body,
            "header": header,
        }

    known = set(funcs)
    edges: dict[str, list[str]] = {}
    callers: dict[str, list[str]] = defaultdict(list)
    external_callees: Counter[str] = Counter()
    for addr, fn in funcs.items():
        resolved, unresolved = [], []
        for callee in fn["callees"]:
            if callee in known:
                resolved.append(callee)
                callers[callee].append(addr)
            else:
                unresolved.append(callee)
                external_callees[callee] += 1
        fn["calls"] = resolved
        fn["unresolved_calls"] = unresolved
        edges[addr] = resolved

    for fn in funcs.values():
        fn["callers"] = sorted(callers.get(fn["address"], []))

    # ---------------- write global files ----------------
    OUT.mkdir(exist_ok=True)
    (OUT / "functions").mkdir(exist_ok=True)

    inventory = []
    for addr in sorted(funcs, key=lambda a: int(a, 16)):
        fn = funcs[addr]
        entry = {k: v for k, v in fn.items() if k not in ("body", "header")}
        entry["coverage"] = coverage.get(addr)
        inventory.append(entry)

    (OUT / "index.json").write_text(
        json.dumps(
            {
                "target_sha256": target,
                "analysis_profile_digest": profile,
                "provider": index.get("provider"),
                "functions_total": len(entries),
                "pseudocode_functions": len(inventory),
                "external_functions": len(external),
                "hash_checked": hash_checked,
                "hash_failures": failures,
                "functions": inventory,
                "external": [
                    {"address": e["address"], "name": e["name"]}
                    for e in sorted(external, key=lambda e: e["address"])
                ],
            },
            indent=1,
        ),
        encoding="utf-8",
    )

    (OUT / "graph.json").write_text(
        json.dumps(
            {
                "edges": {a: funcs[a]["calls"] for a in sorted(funcs)},
                "callers": {a: funcs[a]["callers"] for a in sorted(funcs)},
            },
            indent=1,
        ),
        encoding="utf-8",
    )

    with (OUT / "symbols.csv").open("w", encoding="utf-8") as fh:
        fh.write("address,name,bytes,ranges,scope\n")
        for entry in inventory:
            ranges = ";".join(f"{s}-{e}" for s, e in entry["ranges"])
            scope = entry["coverage"][1] if entry["coverage"] else ""
            fh.write(
                f"{entry['address']},{entry['name']},{entry['size']},{ranges},{scope}\n"
            )

    # normalized per-function sources
    for addr, fn in funcs.items():
        calls = ", ".join(fn["calls"]) or "-"
        port = coverage.get(addr)
        port_line = f"// port: {port[0]} [{port[1]}]\n" if port else ""
        text = (
            "// Reconstructed pseudocode (decompiler output, not original source)\n"
            f"// address: {addr}\n"
            f"// name: {fn['name']}\n"
            f"// ranges: {'; '.join(f'{s}-{e}' for s, e in fn['ranges'])}\n"
            f"// calls: {calls}\n"
            f"{port_line}"
            "\n"
            f"{fn['body']}\n"
        )
        (OUT / "functions" / f"{addr}.c").write_text(text, encoding="ascii", errors="replace")

    # ---------------- index.md ----------------
    total_bytes = sum(fn["size"] for fn in funcs.values())
    lines = [
        "# Full function inventory",
        "",
        f"Target SHA-256: `{target}`  ",
        f"Analysis profile: `{profile}`  ",
        f"Pseudocode functions: **{len(inventory)}**  ",
        f"External functions: **{len(external)}**  ",
        f"Selected body bytes: **{total_bytes}**  ",
        f"Export hashes re-checked: **{hash_checked}**  ",
        f"Hash failures: **{len(failures)}**",
        "",
        "Scope column: `bounded`/`partial` = a documented portable subset exists",
        "in `port/`, not whole-function equivalence. Empty = not implemented.",
        "",
        "| Address | Name | Bytes | Calls | Callers | Scope |",
        "| --- | --- | ---: | ---: | ---: | --- |",
    ]
    for entry in inventory:
        scope = entry["coverage"][1] if entry["coverage"] else ""
        lines.append(
            f"| `{entry['address']}` | {entry['name']} | {entry['size']} | "
            f"{len(entry['calls'])} | {len(entry['callers'])} | {scope} |"
        )
    (OUT / "index.md").write_text("\n".join(lines) + "\n", encoding="utf-8")

    # ---------------- modules.md ----------------
    by_region: dict[int, list[dict]] = defaultdict(list)
    for entry in inventory:
        region = int(entry["address"], 16) >> 16
        by_region[region].append(entry)

    in_degree = sorted(
        inventory, key=lambda e: (-len(e["callers"]), -e["size"])
    )[:25]
    by_size = sorted(inventory, key=lambda e: -e["size"])[:25]
    roots = [e for e in inventory if not e["callers"]]

    modules = [
        "# Engine map from the saved exports",
        "",
        "Mechanical grouping of decompiler output. No original module names,",
        "directory structure or types are recovered. Regions are 64 KiB address",
        "buckets; they follow the original linker layout, not a source tree.",
        "",
        "## Regions",
        "",
        "| Region | Functions | Bytes | Largest functions |",
        "| --- | ---: | ---: | --- |",
    ]
    for region in sorted(by_region):
        bucket = by_region[region]
        top = sorted(bucket, key=lambda e: -e["size"])[:3]
        top_text = ", ".join(f"`{t['address']}` ({t['size']} B)" for t in top)
        modules.append(
            f"| `0x{region:02x}0000` | {len(bucket)} | "
            f"{sum(e['size'] for e in bucket)} | {top_text} |"
        )

    game = [e for e in inventory if int(e["address"], 16) < 0x440000]
    crt = [e for e in inventory if int(e["address"], 16) >= 0x440000]
    game_underscored = sum(1 for e in game if e["name"].startswith("_"))
    crt_underscored = sum(1 for e in crt if e["name"].startswith("_"))

    # Deterministic label propagation over the game-code call graph. Only a
    # coarse, mechanical partition of the exports; not recovered modules.
    game_addrs = {e["address"] for e in game}
    adjacency: dict[str, set[str]] = {a: set() for a in game_addrs}
    for e in game:
        for callee in e["calls"]:
            if callee in game_addrs and callee != e["address"]:
                adjacency[e["address"]].add(callee)
                adjacency[callee].add(e["address"])
    labels = {a: a for a in game_addrs}
    for _ in range(32):
        changed = 0
        for addr in sorted(game_addrs, key=lambda a: int(a, 16)):
            neighbours = adjacency[addr]
            if not neighbours:
                continue
            counts: Counter[str] = Counter(labels[n] for n in neighbours)
            best = min(
                counts,
                key=lambda label: (-counts[label], int(label, 16)),
            )
            if labels[addr] != best:
                labels[addr] = best
                changed += 1
        if changed == 0:
            break
    clusters: dict[str, list[dict]] = defaultdict(list)
    for entry in game:
        clusters[labels[entry["address"]]].append(entry)
    ranked = sorted(
        (c for c in clusters.values() if len(c) > 2),
        key=lambda c: (-sum(e["size"] for e in c), -len(c)),
    )

    modules += ["", "## Code bases", "",
                f"- `0x401000-0x43ffff`: **{len(game)}** functions, "
                f"{sum(e['size'] for e in game)} body bytes, "
                f"{game_underscored} with an underscored CRT-style name.",
                f"- `0x440000-0x45ffff`: **{len(crt)}** functions, "
                f"{sum(e['size'] for e in crt)} body bytes, "
                f"{crt_underscored} with an underscored CRT-style name.",
                "",
                "The address split follows the recovered symbol names: every",
                "function at or above 0x440000 that is named is a CRT or thunk",
                "routine, while the 0x40xxxx-0x43xxxx range contains no",
                "underscored name. Treat the split as an inference.",
                ""]

    modules += ["", "## Most-called game functions (utility candidates)", "",
                "| Address | Name | Bytes | Callers |", "| --- | --- | ---: | ---: |"]
    game_by_callers = sorted(game, key=lambda e: (-len(e["callers"]), -e["size"]))
    for e in game_by_callers[:20]:
        modules.append(
            f"| `{e['address']}` | {e['name']} | {e['size']} | {len(e['callers'])} |"
        )

    modules += ["", "## Largest game functions", "",
                "| Address | Name | Bytes | Calls | Callers |",
                "| --- | --- | ---: | ---: | ---: |"]
    for e in sorted(game, key=lambda e: -e["size"])[:20]:
        modules.append(
            f"| `{e['address']}` | {e['name']} | {e['size']} | "
            f"{len(e['calls'])} | {len(e['callers'])} |"
        )

    modules += ["", "## Game call-graph roots", "",
                "No recovered internal caller; can be real entry points or",
                "vtable/callback targets. Ten largest:", ""]
    game_roots = [e for e in game if not e["callers"]]
    for e in sorted(game_roots, key=lambda e: -e["size"])[:10]:
        modules.append(
            f"- `{e['address']}` {e['name']} — {e['size']} B, {len(e['calls'])} calls"
        )

    modules += ["", "## Imported API surface", ""]
    dlls: dict[str, list[str]] = defaultdict(list)
    for e in external:
        name = e["name"]
        dll, _, symbol = name.partition("::")
        dlls[dll].append(symbol)
    for dll in sorted(dlls, key=lambda d: (-len(dlls[d]), d)):
        symbols = sorted(dlls[dll])
        modules.append(f"- **{dll}** ({len(symbols)}): {', '.join(symbols)}")

    modules += ["", "## Coarse call-graph clusters (game code only)", "",
                "Label propagation over undirected call edges, clusters larger than",
                "two functions, ranked by body bytes. Component labels are address",
                "keys, not recovered module names.", "",
                "| Label | Functions | Bytes | Address span | Largest members |",
                "| --- | ---: | ---: | --- | --- |"]
    for cluster in ranked[:25]:
        start = min(int(e["address"], 16) for e in cluster)
        end = max(int(e["address"], 16) + e["size"] for e in cluster)
        members = sorted(cluster, key=lambda e: -e["size"])[:3]
        member_text = ", ".join(
            f"`{m['address']}` ({m['size']} B)" for m in members
        )
        modules.append(
            f"| `{min(cluster, key=lambda e: int(e['address'], 16))['address']}` | "
            f"{len(cluster)} | {sum(e['size'] for e in cluster)} | "
            f"`0x{start:x}`-`0x{end:x}` | {member_text} |"
        )

    modules += ["", "## Unimplemented game functions by size (roadmap)", "",
                "No documented `port/` counterpart. This is a static inventory, not",
                "a statement that each body must be ported verbatim.", "",
                "| Address | Name | Bytes | Calls | Callers |",
                "| --- | --- | ---: | ---: | ---: |"]
    unimplemented = sorted(
        (e for e in game if not e["coverage"]), key=lambda e: -e["size"]
    )
    for e in unimplemented[:40]:
        modules.append(
            f"| `{e['address']}` | {e['name']} | {e['size']} | "
            f"{len(e['calls'])} | {len(e['callers'])} |"
        )
    modules.append(
        f"\n{len(unimplemented)} of {len(game)} game functions have no documented "
        "port counterpart."
    )

    (OUT / "modules.md").write_text("\n".join(modules) + "\n", encoding="utf-8")

    # ---------------- README.md ----------------
    implemented = sum(1 for e in inventory if e["coverage"])
    readme = f"""# Full source corpus (decompiler-derived)

Generated by `analysis/rea/build_source_corpus.py` from the saved REA/Ghidra
decompile exports. Re-run it instead of editing files here.

## What this is

- `functions/<address>.c` — every recovered function body, with a compact
  header (address, ranges, internal calls, port coverage).
- `index.md` / `index.json` / `symbols.csv` — the complete inventory.
- `graph.json` — call edges and callers for every recovered function.
- `modules.md` — regional, utility and call-graph overview.
- `vm-commands.md` / `vm-commands.json` — generated separately by
  `../build_vm_command_map.py`: the 155-opcode union of the two recovered
  dispatchers, with recovered mnemonics, operand offsets and port coverage.
- `vm-census.md` / `vm-census.json` — generated separately by
  `../build_vm_census.py`: which opcodes block each of the 7,693 scene entry
  labels, measured from a fresh SceneState per label.

## What this is not

- Not the original source. Original C++ files, identifiers, comments and types
  are absent; the executable only references a missing `betrayal.pdb`.
- Not compilable. Pseudocode uses Ghidra types (`undefined4`, `float10`,
  `FUN_...`) and unresolved external symbols.
- Not proof of runtime behavior. Only `port/` is compiled and tested.

## Identity

- Target: `BETRAYAL.EXE`
- SHA-256: `{target}`
- Analysis profile: `{profile}`
- Pseudocode functions: {len(inventory)} of {len(entries)} inventory entries
  ({len(external)} external/import entries have no body).
- Body bytes covered: {total_bytes}
- Export hashes verified: {hash_checked}, failures: {len(failures)}.

## Port coverage

{implemented} of {len(inventory)} functions map to a documented `port/` helper
(`bounded` = recovered algorithm for the documented subset, `partial` = branch,
formula or state subset). These are not completed original functions. The
manual attribution lives in `../IMPLEMENTATION_QUICK_AUDIT.md`.

## Reading order

1. `modules.md` for the layout and the biggest/most-called routines.
2. `index.md` to find an address or an entry point.
3. `functions/<address>.c` for the body; `graph.json` for callers.

The portable, buildable reconstruction remains `port/`. Verify behavior with
`make -C port -j2 all test`.
"""
    (OUT / "README.md").write_text(readme, encoding="utf-8")

    if failures:
        print("HASH FAILURES:", file=sys.stderr)
        for failure in failures:
            print("  " + failure, file=sys.stderr)
        return 1

    print(
        json.dumps(
            {
                "functions": len(inventory),
                "external": len(external),
                "body_bytes": total_bytes,
                "edges": sum(len(v) for v in edges.values()),
                "roots": len(roots),
                "covered_addresses": implemented,
                "hashes_checked": hash_checked,
                "out": str(OUT.relative_to(REA)),
            },
            indent=1,
        )
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
