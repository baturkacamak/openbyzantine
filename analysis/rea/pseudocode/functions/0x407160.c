// rea decompile_all
// name: FUN_00407160
// address: 0x407160
// body: available
// range: 0x407160-0x407253 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


undefined4 FUN_00407160(void)

{
  undefined4 uVar1;
  int local_114;
  uint local_110 [65];
  int local_c;
  undefined4 local_8;
  
  for (local_114 = 0; local_114 < 6; local_114 = local_114 + 1) {
    local_c = DAT_0045d1ac * local_114 + 0xa0;
    local_8 = 0x1aa;
    *(int *)(&DAT_00468d00 + local_114 * 0x10) = local_c;
    *(undefined4 *)(&DAT_00468d04 + local_114 * 0x10) = 0x1aa;
    *(int *)(&DAT_00468d08 + local_114 * 0x10) = DAT_0045d1ac + local_c;
    *(undefined4 *)(&DAT_00468d0c + local_114 * 0x10) = 0x1e6;
  }
  if (DAT_0045d1b0 == 0) {
    FUN_0040c624(local_110,(uint *)s_inventor_0045d1ec,(uint *)s_instrip_0045d1e4,
                 (uint *)&DAT_0045d1dc);
    uVar1 = FUN_00416710((char *)local_110,&DAT_0045d1b0,0,(uint *)0x0,(uint *)0x0);
  }
  else {
    uVar1 = 0;
  }
  return uVar1;
}

