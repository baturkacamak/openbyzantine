// rea decompile_all
// name: FUN_00410607
// address: 0x410607
// body: available
// range: 0x410607-0x410668 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


uint FUN_00410607(void)

{
  int in_EAX;
  int iVar1;
  uint local_10;
  uint local_c;
  uint local_8;
  
  local_8 = local_8 & 0xffff0000;
  for (local_10 = 0; (int)local_10 < 0x15e; local_10 = local_10 + 1) {
    iVar1 = FUN_00410669(local_10,&local_c,(uint *)&DAT_0045e8dc);
    in_EAX = iVar1 + (local_8 & 0xffff);
    local_8 = CONCAT22(local_8._2_2_,(short)in_EAX);
  }
  return CONCAT22((short)((uint)in_EAX >> 0x10),(undefined2)local_8);
}

