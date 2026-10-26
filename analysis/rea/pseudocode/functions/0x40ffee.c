// rea decompile_all
// name: FUN_0040ffee
// address: 0x40ffee
// body: available
// range: 0x40ffee-0x410077 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_0040ffee(void *param_1,undefined2 param_2)

{
  undefined4 uVar1;
  
  _memset(param_1,0,0x40);
  *(undefined2 *)((int)param_1 + 0x14) = param_2;
  *(undefined2 *)((int)param_1 + 2) = 0x14;
  *(undefined1 *)((int)param_1 + 9) = 0x20;
  *(undefined2 *)((int)param_1 + 4) = 10;
  *(undefined4 *)((int)param_1 + 0x2c) = 0xffffffff;
  *(undefined4 *)((int)param_1 + 0x30) = 0xffffffff;
  uVar1 = FUN_0044c2f0(0x18);
  *(undefined4 *)((int)param_1 + 0x18) = uVar1;
  *(undefined4 *)((int)param_1 + 0x1c) = *(undefined4 *)((int)param_1 + 0x18);
  *(undefined4 *)(*(int *)((int)param_1 + 0x18) + 8) = *(undefined4 *)((int)param_1 + 0x18);
  return;
}

