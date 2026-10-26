// rea decompile_all
// name: FUN_0043fc20
// address: 0x43fc20
// body: available
// range: 0x43fc20-0x43fcca inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_0043fc20(int *param_1)

{
  int iVar1;
  int iVar2;
  
  *(undefined4 *)(*param_1 + 0x14) = 0x65;
  (**(code **)(*param_1 + 4))(param_1,1);
  if (*(char *)(param_1[100] + 0x50) != '\0') {
    *(undefined4 *)(*param_1 + 0x14) = 0x3c;
    (**(code **)*param_1)(param_1);
  }
  iVar1 = 0;
  do {
    *(undefined1 *)((int)param_1 + iVar1 + 0xca) = 0;
    iVar2 = iVar1 + 1;
    *(undefined1 *)((int)param_1 + iVar1 + 0xda) = 1;
    *(undefined1 *)((int)param_1 + iVar1 + 0xea) = 5;
    iVar1 = iVar2;
  } while (iVar2 < 0x10);
  param_1[0x3f] = 0;
  param_1[9] = 0;
  *(undefined2 *)((int)param_1 + 0x102) = 1;
  *(undefined1 *)(param_1 + 0x42) = 0;
  *(undefined2 *)(param_1 + 0x41) = 1;
  *(undefined1 *)(param_1 + 0x40) = 0;
  *(undefined1 *)((int)param_1 + 0x101) = 0;
  *(undefined1 *)((int)param_1 + 0x106) = 0;
  *(undefined1 *)((int)param_1 + 0x107) = 0;
  *(undefined1 *)(param_1[100] + 0x50) = 1;
  return;
}

