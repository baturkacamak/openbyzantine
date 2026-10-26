#!/usr/bin/env python3
"""Extract the VM opcode dispatch tables from saved exports.

Recovered dispatchers:
  0x43340a  scene/dialogue command switch (99 cases)
  0x42db22  second command switch over the same opcode space (108 cases)

Reads:
  pseudocode/functions/<dispatcher>.c
  port/src/cc.cpp      opcodes the portable VM handles
  BETRAYAL.EXE         only to resolve referenced C strings (sorted by address)

Writes analysis/rea/reconstruction/vm-commands.{md,json}.

Mnemonic source: the debug-trace branch of a case passes its own name string to
a gated logger. Resolving the referenced .data string recovers names such as
GOTO/REPLY/HOOK. That the string is the command mnemonic is an inference; the
case-to-opcode mapping and the string bytes are observed. Cases without a
trace branch keep `null` instead of a guessed name.
"""

from __future__ import annotations

import json
import re
import struct
from collections import defaultdict
from pathlib import Path

REA = Path(__file__).resolve().parent
ROOT = REA.parent.parent
PSEUDOCODE = REA / "pseudocode/functions"
CC = ROOT / "port/src/cc.cpp"
BINARY = REA / "BETRAYAL.EXE"
OUT = REA / "reconstruction"

DISPATCHERS = [("0x43340a", 4), ("0x42db22", 2)]
CASE_FMT = r"^%scase (0x[0-9a-f]{1,2}|[0-9]{1,3}):"
REF_RE = re.compile(r"&?(?:_?DAT|s_[A-Za-z0-9]+)_([0-9a-f]{8})")
FUN_RE = re.compile(r"\bFUN_([0-9a-f]{8})\b")
LOGGER_RE = re.compile(r"FUN_0040a(?:2c3|615|6e2|719|760)\(")
PORT_CASE_RE = re.compile(r"\bcase (0x[0-9a-f]{1,2}|[0-9]{1,3}):")
FIELD_TYPED_RE = re.compile(
    r"\*\((undefined[124]|ushort|short|char|byte|int|uint|float10) \*\)"
    r"\(param_2 \+ (0x[0-9a-f]+|[0-9]+)\)"
)
FIELD_INDEX_RE = re.compile(r"\bparam_2\[(0x[0-9a-f]+|[0-9]+)\]")
MNEMONIC_RE = re.compile(r"^[A-Z][A-Z0-9_]{1,15}$")


def pe_sections(binary: bytes):
    pe = struct.unpack_from("<I", binary, 0x3C)[0]
    count = struct.unpack_from("<H", binary, pe + 6)[0]
    optional_size = struct.unpack_from("<H", binary, pe + 20)[0]
    base = struct.unpack_from("<I", binary, pe + 24 + 28)[0]
    sections = []
    for i in range(count):
        pos = pe + 24 + optional_size + i * 40
        _, rva, size, offset = struct.unpack_from("<IIII", binary, pos + 8)
        sections.append((base + rva, size, offset))
    return sections


def parse_cases(path: Path, indent: int) -> dict[int, str]:
    pattern = re.compile(CASE_FMT % (" " * indent))
    closing = " " * indent + "}"
    cases: dict[int, str] = {}
    current: int | None = None
    ended = False
    body: list[str] = []
    for line in path.read_text(encoding="utf-8", errors="replace").splitlines():
        match = pattern.match(line)
        if match:
            if current is not None:
                cases[current] = "\n".join(body)
            current = int(match.group(1), 0)
            ended = False
            body = []
        elif current is not None and not ended:
            if line == closing:
                # The switch closes at the case-label indent; anything after it
                # is function tail, not part of the last case.
                ended = True
                continue
            body.append(line)
    if current is not None:
        cases[current] = "\n".join(body)
    return cases


def main() -> int:
    binary = BINARY.read_bytes()
    sections = pe_sections(binary)

    def read_string(address: int, limit: int = 40) -> str | None:
        for start, size, offset in sections:
            if start <= address < start + size:
                pos = offset + address - start
                end = binary.find(b"\x00", pos, pos + limit)
                if end < 0:
                    return None
                try:
                    text = binary[pos:end].decode("ascii")
                except UnicodeDecodeError:
                    return None
                if len(text) < 2 or any(ord(c) < 0x20 or ord(c) > 0x7E for c in text):
                    return None
                return text
        return None

    width_rank = {
        "char": 1, "byte": 1, "undefined1": 1,
        "short": 2, "ushort": 2, "undefined2": 2,
        "int": 4, "uint": 4, "undefined4": 4, "float10": 10,
    }

    def fields(case_body: str) -> list[dict]:
        # One entry per byte offset, keeping the widest observed access width.
        found: dict[int, str] = {}
        for match in FIELD_TYPED_RE.finditer(case_body):
            offset, width = int(match.group(2), 0), match.group(1)
            if width_rank[width] > width_rank.get(found.get(offset, "char"), 0):
                found[offset] = width
        for match in FIELD_INDEX_RE.finditer(case_body):
            offset = int(match.group(1), 0)
            found.setdefault(offset, "byte")
        return [
            {"offset": "0x%x" % offset, "width": found[offset]}
            for offset in sorted(found)
        ]

    def mnemonic(case_body: str) -> tuple[str | None, str | None]:
        candidates: list[tuple[int, str, int]] = []
        for match in REF_RE.finditer(case_body):
            address = int(match.group(1), 16)
            text = read_string(address)
            if not text or not MNEMONIC_RE.match(text):
                continue
            # Prefer strings passed to a debug logger, then the first ref.
            prefix = case_body[max(0, match.start() - 24):match.start()]
            priority = 0 if LOGGER_RE.search(prefix) else 1
            candidates.append((priority, text, address))
        if not candidates:
            return None, None
        candidates.sort(key=lambda item: (item[0], item[2]))
        return candidates[0][1], hex(candidates[0][2])

    tables: dict[str, dict[int, dict]] = {}
    for dispatcher, indent in DISPATCHERS:
        cases = parse_cases(PSEUDOCODE / f"{dispatcher}.c", indent)
        table = {}
        for opcode, body in cases.items():
            name, name_address = mnemonic(body)
            table[opcode] = {
                "opcode": "0x%02x" % opcode,
                "int": opcode,
                "name": name,
                "name_address": name_address,
                "callees": sorted(
                    {"0x%x" % int(m.group(1), 16) for m in FUN_RE.finditer(body)}
                ),
                "statements": body.count(";"),
                "fields": fields(body) if dispatcher == "0x43340a" else [],
            }
        tables[dispatcher] = table

    cc_text = CC.read_text()
    port_opcodes = {
        int(m.group(1), 0) for m in PORT_CASE_RE.finditer(cc_text)
    }

    union = sorted(set().union(*(t.keys() for t in tables.values())))
    merged = []
    for opcode in union:
        row = {"opcode": "0x%02x" % opcode, "int": opcode, "name": None,
               "dispatchers": [], "callees": [], "port": opcode in port_opcodes}
        for dispatcher in tables:
            entry = tables[dispatcher].get(opcode)
            if entry:
                row["dispatchers"].append(dispatcher)
                if entry["name"] and not row["name"]:
                    row["name"] = entry["name"]
                    row["name_address"] = entry["name_address"]
                row["callees"] = sorted(set(row["callees"]) | set(entry["callees"]))
                if entry["fields"] and not row.get("fields"):
                    row["fields"] = entry["fields"]
        merged.append(row)

    named = [r for r in merged if r["name"]]
    supported = [r for r in merged if r["port"]]
    port_only = sorted(
        op for op in port_opcodes if 0x30 <= op <= 0xFF and op not in union
    )

    (OUT / "vm-commands.json").write_text(
        json.dumps(
            {
                "dispatchers": {
                    name: {
                        "cases": len(table),
                        "named": sum(1 for e in table.values() if e["name"]),
                        "commands": list(table.values()),
                    }
                    for name, table in tables.items()
                },
                "union_opcodes": len(merged),
                "named": len(named),
                "port_supported": len(supported),
                "not_in_port": len(merged) - len(supported),
                "commands": merged,
                "port_only_opcodes": ["0x%02x" % op for op in port_only],
            },
            indent=1,
        ),
        encoding="utf-8",
    )

    md = [
        "# VM command map from the recovered dispatchers",
        "",
        "Extracted from `0x43340a` (scene/dialogue command switch) and `0x42db22`",
        "(second switch over the same opcode space). `name` is the string the case's",
        "debug-trace branch passes to a gated logger; the reading that it is the",
        "command mnemonic is an inference. Cases without a trace branch are marked",
        "`—` rather than guessed. `port` = the opcode has a branch in `port/src/cc.cpp`.",
        "",
        f"- Opcodes in the union: **{len(merged)}**",
        f"- Mnemonics recovered: **{len(named)}**",
        f"- Handled by the portable VM: **{len(supported)}**",
        f"- Not handled: **{len(merged) - len(supported)}**",
        f"- Port opcodes outside the recovered dispatchers: "
        f"{', '.join('`0x%02x`' % o for o in port_only) or 'none'}",
        "",
        "| Opcode | Mnemonic | 0x43340a | 0x42db22 | Port | Fields (0x43340a) | Callees |",
        "| --- | --- | :---: | :---: | :---: | --- | --- |",
    ]
    for row in merged:
        callees = ", ".join(f"`{c}`" for c in row["callees"][:5])
        if len(row["callees"]) > 5:
            callees += ", …"
        fields = ", ".join(
            f"+{f['offset']}" for f in row.get("fields", [])
        )
        md.append(
            f"| `{row['opcode']}` | {row['name'] or '—'} | "
            f"{'yes' if '0x43340a' in row['dispatchers'] else ''} | "
            f"{'yes' if '0x42db22' in row['dispatchers'] else ''} | "
            f"{'yes' if row['port'] else '**no**'} | {fields or '—'} | "
            f"{callees or '—'} |"
        )

    missing_names = [r for r in merged if not r["name"]]
    md += [
        "",
        "## Opcodes without a recovered mnemonic",
        "",
        ", ".join(f"`{r['opcode']}`" for r in missing_names),
        "",
        "These cases do not pass a name string to the trace logger. They remain",
        "identified by address/case only; any mnemonic in a report or the port is",
        "either port-derived or still unknown.",
    ]
    (OUT / "vm-commands.md").write_text("\n".join(md) + "\n", encoding="utf-8")

    print(
        json.dumps(
            {
                "dispatchers": {name: len(t) for name, t in tables.items()},
                "union": len(merged),
                "named": len(named),
                "port_supported": len(supported),
                "port_only": len(port_only),
            }
        )
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
