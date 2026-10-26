// rea decompile_all
// name: FUN_00433384
// address: 0x433384
// body: available
// range: 0x433384-0x433409 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


undefined4 __cdecl FUN_00433384(int param_1)

{
  ushort uVar1;
  undefined4 uVar2;
  uint local_c;
  
  uVar1 = FUN_00436652(param_1,0);
  uVar1 = FUN_00408bb2(uVar1);
  local_c = (uint)uVar1;
  if (((int)*(char *)(*(int *)(&DAT_00486f90 + local_c * 4) + 4) == (uint)DAT_00476338) ||
     ((int)*(char *)(*(int *)(&DAT_00486f90 + local_c * 4) + 4) + 1U == (uint)DAT_00476338)) {
    uVar2 = 1;
  }
  else {
    uVar2 = 0;
  }
  return uVar2;
}

