# VM opcode 0x7d (RANDOM) and the CRT stream

25 October 2026. Static recovery from saved REA/Ghidra exports. No original
execution and no new decompilation session.

Target `analysis/rea/BETRAYAL.EXE` SHA-256
`3b6191fd1c7c590ec3e04298f23742a94366eb1805011ed4ca3add8ad5963226`,
profile `e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9`.

## Why this opcode

`reconstruction/vm-census.md` traces every label of all 44 extracted scenes from
a fresh `SceneState`. `0x7d` was the 13th most frequent first blocker (84
labels) and, unlike the top blockers, it writes the script variable table
directly, so it can be modelled inside the existing VM instead of needing the
actor/renderer subsystem.

## Observed

`0x42db22` case `0x7d` (and the same shape in `0x43340a`):

```
range = (type[operand 1] == 2)   ? scene_variable[value(operand 1)]
      : (type[operand 1] == 0x40) ? constant(value(operand 1))
      : 0xffff;
value = FUN_00431e89(range);          // 0x431e89
scene_variable[value(operand 0)] = value;
```

- The type check reads the record's per-operand type byte (`record[2]` is the
  type of operand 1), not the operand value.
- `0x431e89(range)`: if `range == 0` the low word is 0 and `_rand` is **not**
  called; otherwise it calls `_rand()` and returns the low 16 bits of
  `rand() % range`.
- `0x44d070` is `_rand` (Ghidra header: "Library Function - Single Match,
  _rand, Visual Studio 1998 Release"):
  `state = state*0x343fd + 0x269ec3; return (state & 0x7fff0000) >> 16`.
  `0x44d060` is `_srand` and writes the same global `0x462ef8`.
- Seed sites: `0x415a4e` calls `0x44d060(0xbeef)` on one startup path;
  `0x42a6aa` seeds from `0x44d4d0`, which reads `GetLocalTime` and converts it
  through `0x451cb0`, then calls `_rand()` once.
- Both dispatchers route the case through their trace logger; `0x43340a`
  passes the literal `"RANDOM"` at `0x45d420`.

## Portable implementation

- `SceneState::random_state` is `std::optional<std::uint32_t>` for the CRT
  global; it is unset by default, because neither seed site can be assumed.
- `SceneTrace::unresolved_random` reports that `0x7d` was reached without a
  known stream. The command is then not executed and the trace stops, matching
  the existing "do not invent carried state" rule.
- With a seed supplied, `port/src/cc.cpp` advances the recovered LCG exactly and
  stores `rand() % range`. A zero range stores 0 without advancing the stream.
  Type `2` reads another scene variable (raw operand key, as in `0x67`), type
  `0x40` uses the constant, any other type uses `0xffff`.
- Because the range variable is resolved before the seed check, a missing range
  variable reports `unresolved_variable` rather than `unresolved_random`.

New tests in `port/tests/scene_state.cpp` check, with seed `0xbeef`:

| Draw (range 100) | Stream after | Value |
| --- | --- | ---: |
| 1 | `0x6fa8ddf6` | 84 |
| 2 | `0x26035ce1` | 31 |
| 3 | `0x896b4c20` | 11 |
| 4 | `0xc87d3a63` | 57 |

Also covered: unseeded stop leaves the variable table untouched; range from a
variable (`rand()%7 == 3`); zero range stores 0 and leaves the stream at
`0x6fa8ddf6`; a missing range variable reports `unresolved_variable`; a
non-constant/non-variable range operand reads `0xffff` (`28584`).

## Effect on the census

`0x7d` no longer appears as an unsupported opcode. Of its 84 blocked labels, 69
now stop on `unresolved_variable` (their range operand is itself carried state)
and 15 on `unresolved_random`; none previously completed, because the command
was unimplemented. Aggregate after the change: 7,693 labels, 2,357 completed,
1,799 unresolved variable, 15 unresolved RNG seed, 3,522 unsupported opcode.

## Not claimed

- Which startup path `0x415a4e` represents is not established, so `0xbeef` is
  not applied automatically; the local-time seed is host-dependent and not
  reproduced.
- The CRT implementation is recovered from its export and header, not from a
  runtime trace. `_rand` sequence equality with an original session is not
  verified.
- `0x7d`'s per-scene variable mapping is modelled by the raw operand key, like
  the existing `0x67`; the original process-global remap is still outside this
  VM layer.
