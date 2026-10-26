# Dialogue/conversation state (0xab, 0xb0/0xb1, 0xb6)

25 October 2026. Static findings only; no code change in this step.

> **Superseded for the state model.** `DIALOGUE_RECORDS.md` implements and
> verifies the record array, the four engine tables, the channel stacks and
> these four opcodes. The initial-state claim below ("unknown by default") is
> corrected there: `0x408662` allocates the records with `+0x00 = 0`, and the
> id space is the global `engine/SHOW.LCI` order (126 records on the shipped
> CD1), not a per-scene table.

## Observed

`0xab` is the conversation-line command. `0x42db22` case `0xab` resolves:

- operand 0 through `0x408bb2`: maps a name id from `DAT_00485400` (name at
  `+0`) to its index in the active list `DAT_00482540` (up to `0x15e` entries,
  case-insensitive compare; falls back to 0 and logs through `0x40ac43`).
- operand 1 = mode (0, 1 or 2).
- operand 2 through `0x408c81`: maps a name id from `0x47a2b0` to an index in
  the ten-entry list `0x47a8b0` (same compare/fallback pattern).

Then `0x4077b2` starts a line and `0x407933` ends/handles it:

- both read the per-id record pointer at `DAT_00486f90[id]`;
- `0x4077b2` sets the record's first byte to 1, pushes the id onto a per-channel
  stack (`DAT_00468cc0` count, `DAT_00468d78` + channel*0x578 + index*4), and
  updates `DAT_0045d1c8`/`DAT_0045d1c4` when the channel is the current one;
- `0x407933` is the matching removal path.

`0xb0`/`0xb1` branch directly on that same per-id dialogue byte
(`**(byte **)(&DAT_00486f90 + id*4)`) and jump to `DAT_004654e0[label]`.
`0xb6` calls the same start helper plus `0x407ad5`.

## Consequence

`0xab` (310 blocked labels), `0xb0` (153), `0xb1` (140), `0xb6` (91) and `0xbd`
(110) all depend on one missing model: the per-id dialogue record
(`DAT_00486f90`) plus the per-channel active stack. They cannot be modelled as
fire-and-forget requests because `0xb0`/`0xb1` branch on that byte.

## Next implementation step

1. Model `SceneState::dialogue` as `id -> {byte0 state, ...}` plus the channel
   stack, with the record's initial state unknown by default (the original
   fills it from scene resources, `0x4117a6`-style tables).
2. Implement `0xab` start/stop (mode 0/1/2), `0xb6`, and then `0xb0`/`0xb1`
   as real branches; a trace without a known dialogue record must stop with an
   explicit unresolved-dialogue flag, never guess the branch.
3. Only then the remaining media commands (`0x6c` 236, `0xcc` 178) and `0xb9`.

No runtime comparison exists for any of this; the findings are static.
