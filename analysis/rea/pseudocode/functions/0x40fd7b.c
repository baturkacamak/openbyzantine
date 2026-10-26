// rea decompile_all
// name: FUN_0040fd7b
// address: 0x40fd7b
// body: available
// range: 0x40fd7b-0x40fe6f inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


undefined2 FUN_0040fd7b(void)

{
  DWORD DVar1;
  uint local_14;
  undefined2 local_8;
  
  local_8 = 0;
  FUN_004102ae();
  if (DAT_00460660 == 0) {
    if (DAT_004603d0 == 0) {
      DAT_0045c05c = 0;
    }
    for (local_14 = 0; (int)local_14 < 0x15e; local_14 = local_14 + 1) {
      if ((((*(int *)(&DAT_0047dd70 + local_14 * 4) != 0) &&
           (*(char *)(*(int *)(&DAT_0047dd70 + local_14 * 4) + 9) == '\0')) &&
          ((*(char *)(*(int *)(&DAT_0047dd70 + local_14 * 4) + 8) == '\x02' ||
           (*(char *)(*(int *)(&DAT_0047dd70 + local_14 * 4) + 8) == '\x03')))) &&
         (DVar1 = FUN_0040fe70(local_14,(int *)(&DAT_004649e8 + local_14 * 4)), DVar1 == 0)) {
        local_8 = 1;
      }
    }
  }
  else {
    local_8 = 0;
  }
  return local_8;
}

