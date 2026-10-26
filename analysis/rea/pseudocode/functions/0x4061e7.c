// rea decompile_all
// name: FUN_004061e7
// address: 0x4061e7
// body: available
// range: 0x4061e7-0x4062bb inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


int __cdecl FUN_004061e7(int param_1,int param_2)

{
  byte bVar1;
  int local_18;
  int local_14;
  int local_10;
  
  local_10 = 0;
  local_18 = 0;
  for (local_14 = 0; *(char *)(local_14 + param_1) != '\0'; local_14 = local_14 + 1) {
    if (*(char *)(local_14 + param_1) == '\n') {
      if (local_10 < local_18) {
        local_10 = local_18;
      }
      local_18 = 0;
    }
    else {
      bVar1 = *(byte *)(local_14 + param_1);
      if (0x7f < bVar1) {
        bVar1 = FUN_00405bf4();
      }
      local_18 = local_18 +
                 (uint)*(ushort *)
                        ((&DAT_00487518)[param_2 * 0x12] + 4 + (uint)(byte)(bVar1 - 0x20) * 8);
    }
  }
  if (local_10 <= local_18) {
    local_10 = local_18;
  }
  return local_10;
}

