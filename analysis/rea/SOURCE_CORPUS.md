# Full source corpus and VM opcode map

25 October 2026. Mechanical extraction from the saved REA/Ghidra exports plus
the preserved target bytes. No new decompilation, no original execution and no
binary session.

Target `analysis/rea/BETRAYAL.EXE` SHA-256
`3b6191fd1c7c590ec3e04298f23742a94366eb1805011ed4ca3add8ad5963226`,
profile `e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9`.

## What was produced

Two deterministic generators:

| Script | Output |
| --- | --- |
| `build_source_corpus.py` | `reconstruction/` corpus, inventory, call graph, engine map |
| `build_vm_command_map.py` | `reconstruction/vm-commands.{md,json}` |

`reconstruction/`:

- `functions/<address>.c` — all **1,237** pseudocode bodies with a compact
  header (address, ranges, internal calls, existing port coverage).
- `index.md` / `index.json` / `symbols.csv` — complete inventory.
- `graph.json` — 3,406 resolved call edges, per-function callers.
- `modules.md` — regions, code-base split, utilities, call-graph clusters,
  imported API list, unimplemented-body roadmap.
- `README.md` — provenance and limits.

Every written export hash was re-computed and compared with
`pseudocode/decompile-index.json`: **1,237 / 1,237 match**, zero failures.
The 160 `external` inventory entries are import thunks and have no body; they
are listed but not duplicated.

Observed totals: 1,237 bodies, 325,382 body bytes, 3,406 internal call edges,
206 functions with no recovered internal caller, 31 addresses with a documented
`port/` counterpart.

## Code bases

| Range | Functions | Body bytes | Underscored CRT-style names |
| --- | ---: | ---: | ---: |
| `0x401000-0x43ffff` | 840 | 249,250 | 0 |
| `0x440000-0x45ffff` | 397 | 76,132 | 144 |

`0x44d530` is `entry`, and the upper range contains `_memcpy`, `_floor`,
`__nextafter`, `___crtCompareStringW`, the `_except`/catch machinery and the
`__controlfp` helpers — i.e. the statically linked MSVC CRT. The lower range
contains no underscored CRT name. The split is therefore a strong inference
from symbol names and the entry point, not a recovered linker map. It is not a
game/MFC split: 28 recovered names in the lower range are MFC/ATL members
(`CSplitterWnd::IsTracking`, `CDataBoundProperty::…`), so part of that range is
statically linked MFC.

## VM opcode map

Two switches operate over one opcode space:

- `0x43340a` — 99 cases, the scene/dialogue command switch that also updates
  the script pointer, call stack and reply state.
- `0x42db22` — 108 cases, a second command switch over the same space.

Union: **155 opcodes**. The portable VM in `port/src/cc.cpp` has a branch for
**28**; **127** have no port counterpart. One port opcode (`0xe1`) is not in
either switch and is presumably dispatched elsewhere.

### Recovered mnemonics (39)

`GOTO, REPLY, HOOK, THOOK, ON, OFF, SET, RUDE, DONE, MOOD, UNSPEAK, BACK, IFON,
IFOFF, IFSET, IFREPLY, IFGOTO, TOFF, TON, IFTOFF, IFTON, SPEAK, INVGOTO,
INVREPLY, INVSET, INVOFF, INVON, INVTOFF, INVTON, CURSGOTO, CURSREPL, CURSSET,
CURSOFF, CURSON, CURSTOFF, CURSTON, BACKGRND, CONVERSE, NODE`

Method: each case's debug-trace branch passes its own name literal to a gated
logger (`0x40a719`, `0x40a760`, `0x40a6e2`, `0x40a615`, `0x40a2c3`), and the
string bytes sit in `.data` (for example `0x4610d8 = "GOTO"`,
`0x4610e0 = "REPLY"`, `0x4610e8 = "HOOK"`, `0x4610f0 = "THOOK"`,
`0x46110c = "SET"`, `0x4611f8 = "BACKGRND"`). The byte strings and the
case-to-opcode mapping are **observed**; reading them as the command mnemonic
is an **inference** supported by their position in the trace path.

The 116 cases without a trace name remain `—`. They were deliberately not
guessed, and the table is guarded so that a case only gets a name from a string
it actually references.

Operand offsets are also extracted mechanically for the `0x43340a` cases, where
the second argument is the command record (`param_2`): the table lists each
`param_2 + N` / `param_2[N]` access with its widest observed width. This is a
lower bound on the operand layout, not a full argument decode; `0x42db22`
receives a script record instead, so its fields come from the per-command
handlers and are not extracted here.

## Limits

- This is decompiler output, not original source: no original identifiers,
  types, comments or files are recovered, and nothing here is compilable.
- `port/` remains the only buildable reconstruction and the only tested
  behavior.
- The 160 import entries and the MFC/CRT bodies are not re-derived.
- Mnemonics describe the debug trace, not argument formats or side effects.
  Argument layout still has to be recovered per command.
- No runtime comparison was performed.

## Reproduce

```sh
python3 analysis/rea/build_source_corpus.py
python3 analysis/rea/build_vm_command_map.py
make -C port -j2 all test
```
