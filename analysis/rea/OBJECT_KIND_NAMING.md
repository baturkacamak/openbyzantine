# Naming three object kinds from existing evidence

29 October 2026. User requested a small implemented example of replacing
unexplained numeric codes, followed by an explanation of the investigation.

## Scope and names

| Raw kind | C++ constant in `byzantine::object_kind` | Established distinction |
| --- | --- | --- |
| `0x12` | `kApplyRecord` | Normal cursor `apply`; target is a single dialogue/inventory record |
| `0x13` | `kApplyChain` | Normal cursor `apply`; target is a dialogue chain of acceptable records |
| `0x14` | `kTalkChain` | Normal cursor `talk`; target is a dialogue chain of acceptable records |

These are descriptive reconstruction names, not recovered original C++ symbols.
`TalkChain` describes the normal cursor and target representation; it does not
claim that every such object's script starts a conversation. In particular,
the alternate klio cursor table has no art for slot `0x14`.

## Evidence and method

1. Follow the field, rather than searching all occurrences of the number:
   `SceneObjectState::kind` is the original object's byte at `+0x09`.
2. Follow creation: `0x42db22`, case `0x72`, stores operand 7 directly in
   `+0x10` for kinds `0x13` and `0x14`; other kinds resolve the CI name to a
   record id. The portable counterpart is `port/src/cc.cpp`.
3. Follow use: `0x407c5a` handles all three together but compares `0x12` with
   the selected record directly, whereas `0x13` and `0x14` walk the chain at
   `DAT_00486a10`. All three reject a missing target. The counterpart is
   `item_use_compatible` in `port/src/inventory.cpp`.
4. Corroborate presentation: `0x40c329` uses the object's kind as cursor id
   and selects held-item hot art for these three compatible types. Read the
   original normal cursor table at VA `0x45dbe8`, stride `0x24`, directly
   from the PE file: slots `0x12` and `0x13` point to `apply`, `0x14` to `talk`.
   Existing background: `ITEM_USE.md` and `CURSORS.md`.

SHA-256 of the actual target and selected saved exports was checked against
`pseudocode/decompile-index.json` during this change:

| Input | SHA-256 |
| --- | --- |
| `analysis/rea/BETRAYAL.EXE` | `3b6191fd1c7c590ec3e04298f23742a94366eb1805011ed4ca3add8ad5963226` |
| `functions/0x407c5a.c` | `862c7fc71bc4f5da8ec6b020cc36e83583d5a8d5f867129ce5cd0addc078bb26` |
| `functions/0x40c329.c` | `5a9e9344f5661475134cfcfc8f708404831951a0bc006244f506ddeabcfa7491` |
| `functions/0x42db22.c` | `8df50fc5339a585b431bd3f8bdaea0cf93e60129fc942669de9059dd3fb130a3` |

This is static evidence from the saved Ghidra exports and original table data;
no new decompilation or original-game execution was performed.

## Implementation and validation

`port/include/byzantine/cc.hpp` defines three `std::uint8_t` constants.
Creation, compatibility and cursor selection/table entries use those names.
The kind field remains a raw byte, including support for unnamed kinds; the
numeric values, script interpretation, save layout and rules are unchanged.
Byte constants make this a bounded migration without converting every kind
or introducing casts for a partially named enum.

Existing tests deliberately retain literal inputs `0x12`, `0x13`, `0x14` as
independent format fixtures. They check CI resolution versus direct chain id,
correct/wrong/absent selected items, chain membership, and held-item cursors.
No new tests duplicating constant definitions were added.

Validation: `make -C port -j2 all test` completed successfully (exit 0),
including scene-state, inventory and cursor tests. `git diff --check` passes.
Log: `port/output/object-kind-naming/build-test.log`.
Prior user run retained: `run-20261028T224030.673870Z-rlru45kf`.

## Limits

Only these three kinds are named. Other object kinds, opcodes, offsets, flags
and record states require their own context and evidence; identical numbers
in those domains must not be replaced with these constants. This change does
not implement or claim any additional game behavior or exhaustive playthrough.
