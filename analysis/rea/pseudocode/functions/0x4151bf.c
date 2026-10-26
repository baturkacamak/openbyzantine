// rea decompile_all
// name: FUN_004151bf
// address: 0x4151bf
// body: available
// range: 0x4151bf-0x4152dc inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


undefined4 __cdecl FUN_004151bf(int param_1)

{
  int iVar1;
  
  if (DAT_00486884 != (int *)0x0) {
    do {
      iVar1 = (**(code **)(*DAT_00486884 + 8))(DAT_00486884);
    } while (iVar1 != 0);
    DAT_00486884 = (int *)0x0;
  }
  if (DAT_004869d4 != (int *)0x0) {
    do {
      iVar1 = (**(code **)(*DAT_004869d4 + 8))(DAT_004869d4);
    } while (iVar1 != 0);
    DAT_004869d4 = (int *)0x0;
  }
  if (param_1 != 0) {
    iVar1 = (**(code **)(*DAT_00486a04 + 0x4c))(DAT_00486a04);
    if (iVar1 != 0) {
      return 0;
    }
    if (DAT_00486a04 != (int *)0x0) {
      do {
        iVar1 = (**(code **)(*DAT_00486a04 + 8))(DAT_00486a04);
      } while (iVar1 != 0);
      DAT_00486a04 = (int *)0x0;
    }
  }
  return 1;
}

