// rea decompile_all
// name: FUN_00410fee
// address: 0x410fee
// body: available
// range: 0x410fee-0x4110e4 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_00410fee(undefined4 *param_1)

{
  LPVOID pvVar1;
  LPVOID pvVar2;
  LPVOID local_18;
  int local_10;
  LPVOID local_8;
  
  pvVar1 = (LPVOID)*param_1;
  local_8 = *(LPVOID *)((int)pvVar1 + 0xcc);
  for (local_10 = 0; local_10 < 0x14; local_10 = local_10 + 1) {
    if (*(int *)((int)pvVar1 + local_10 * 4 + 0x2c) != 0) {
      FUN_0044ac40(*(LPVOID *)((int)pvVar1 + local_10 * 4 + 0x2c));
    }
  }
  while (local_8 != (LPVOID)0x0) {
    for (local_10 = 0; local_10 < 5; local_10 = local_10 + 1) {
      local_18 = *(LPVOID *)((int)local_8 + local_10 * 4);
      while (local_18 != (LPVOID)0x0) {
        pvVar2 = *(LPVOID *)((int)local_18 + 4);
        FUN_0044ac40(local_18);
        local_18 = pvVar2;
      }
    }
    pvVar2 = *(LPVOID *)((int)local_8 + 0x28);
    FUN_0044ac40(local_8);
    local_8 = pvVar2;
  }
  FUN_0044ac40(pvVar1);
  *param_1 = 0;
  return;
}

