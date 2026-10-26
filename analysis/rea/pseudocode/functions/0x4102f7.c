// rea decompile_all
// name: FUN_004102f7
// address: 0x4102f7
// body: available
// range: 0x4102f7-0x41045a inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_004102f7(int param_1)

{
  int *piVar1;
  LPVOID pvVar2;
  int *local_14;
  LPVOID local_8;
  
  if (*(int *)(&DAT_0047dd70 + param_1 * 4) != 0) {
    *(undefined1 *)(*(int *)(&DAT_0047dd70 + param_1 * 4) + 9) = 0;
  }
  if (*(int *)(&DAT_004649e8 + param_1 * 4) != 0) {
    local_14 = *(int **)(*(int *)(&DAT_004649e8 + param_1 * 4) + 0x18);
    if ((local_14 != (int *)0x0) && (local_14[2] != 0)) {
      *(undefined4 *)(local_14[2] + 4) = 0;
    }
    while (local_14 != (int *)0x0) {
      if (*local_14 != 0) {
        FUN_0044ac40((LPVOID)*local_14);
      }
      piVar1 = (int *)local_14[1];
      FUN_0044ac40(local_14);
      local_14 = piVar1;
    }
    local_8 = *(LPVOID *)(*(int *)(&DAT_004649e8 + param_1 * 4) + 0x34);
    while (local_8 != (LPVOID)0x0) {
      pvVar2 = *(LPVOID *)((int)local_8 + 8);
      FUN_0044ac40(local_8);
      local_8 = pvVar2;
    }
    if (*(int *)(*(int *)(&DAT_004649e8 + param_1 * 4) + 0x38) != 0) {
      FUN_0041cd4b(*(int *)(&DAT_004649e8 + param_1 * 4));
    }
    if (*(int *)(*(int *)(&DAT_004649e8 + param_1 * 4) + 0x3c) != 0) {
      FUN_00432602(*(int *)(&DAT_004649e8 + param_1 * 4));
    }
    FUN_0044ac40(*(LPVOID *)(&DAT_004649e8 + param_1 * 4));
    *(undefined4 *)(&DAT_004649e8 + param_1 * 4) = 0;
  }
  return;
}

