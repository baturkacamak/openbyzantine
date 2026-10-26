// rea decompile_all
// name: FUN_004288e5
// address: 0x4288e5
// body: available
// range: 0x4288e5-0x428a15 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void FUN_004288e5(void)

{
  int iVar1;
  int iVar2;
  int local_14;
  uint *local_8;
  
  iVar1 = FUN_00422a90();
  for (local_8 = DAT_0046019c; local_8 != (uint *)0x0; local_8 = (uint *)local_8[0x16]) {
    if (local_8[0x15] == 0) {
      FUN_0042826e(local_8);
    }
    else if ((local_8[0xc] != 0) || (iVar2 = FUN_0041d440(2), iVar2 == 1)) {
      (**(code **)(**(int **)(local_8[0x15] + 8) + 0x30))(*(int **)(local_8[0x15] + 8),0,0,1);
    }
  }
  for (local_14 = 0; local_14 < 0x15e; local_14 = local_14 + 1) {
    if ((*(int *)(&DAT_00486220 + local_14 * 4) != 0) &&
       (*(int *)(*(int *)(&DAT_00486220 + local_14 * 4) + 0x34) != 0)) {
      if ((*(char *)(*(int *)(&DAT_0047a8e0 + local_14 * 4) + 8) != '\x02') || (iVar1 == -1)) {
        FUN_00427de1(local_14,0);
      }
      if (*(int *)(*(int *)(&DAT_00486220 + local_14 * 4) + 0x14) == 0) {
        *(undefined4 *)(*(int *)(&DAT_00486220 + local_14 * 4) + 0x34) = 0;
      }
    }
  }
  return;
}

