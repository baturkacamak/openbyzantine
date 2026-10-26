# REA provenance review — 25 October 2026

Scope: audit existing project evidence; no new binary analysis or session.

Observed:
- `rea.sh` invokes REA CLI with a configured Ghidra/Java environment.
- `session.json` contains 6 evidence records; `mcp-session.json` contains 38
  (25 procedure_pseudo_code, 1 read_bytes, 3 procedure_callers, 4 xrefs,
  4 decompile_all, 1 search_strings). Both identify Ghidra 12.1.4 and the
  same target/profile as the decompile index.
- SHA-256 of BETRAYAL.EXE matches the index. All 1,237 written function
  exports match their indexed SHA-256; 160 additional entries are external.
- README documents initial CLI use and MCP awaiting restart. handoff.md:143
  explicitly reports direct MCP use in a subsequent session.
- These exports contain analysis operations/results, not an MCP wire transcript;
  their entries/workflow_entries are empty. The filename alone does not prove
  transport. A bounded search of locally available Codex session function-call
  records found no identifiable historical REA MCP tool invocation. This does
  not prove absence: those historical transcripts may be unavailable or stored
  under a different tool naming/log format.

Assessment: substantial, internally consistent evidence supports REA/Ghidra
analysis being used for this reconstruction. Direct MCP use is reported in the
handoff and compatible with the saved export, but is not independently proven
by a retained protocol/call transcript in this audit. CLI, saved exports and
focused disassembly also contributed; claiming every step used live MCP would
be unsupported. Hashes establish local consistency, not external attestation
of historical tool execution or gameplay correctness.

Validation: `make -C port -j2 all test` exits 2. Build is current; scene-state
and variable-metadata pass; panorama-test stops at `projection reset camera`.
No implementation changed. Existing failure remains unresolved.
