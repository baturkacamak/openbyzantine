// rea decompile_all
// name: FUN_00418bcf
// address: 0x418bcf
// body: available
// range: 0x418bcf-0x418c91 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_00418bcf(int param_1)

{
  int iVar1;
  tagRECT local_20;
  HWND local_10;
  undefined4 local_c;
  int *local_8;
  
  local_8 = (int *)FUN_00418040(param_1);
  iVar1 = FUN_00416220();
  if (iVar1 == 0) {
    local_10 = (HWND)FUN_004161f6();
    GetClientRect(local_10,&local_20);
    ClientToScreen(local_10,(LPPOINT)&local_20);
    local_20.right = local_20.left + 0x280;
    local_20.bottom = local_20.top + 0x1e0;
    local_c = (**(code **)(*local_8 + 0x14))(local_8,0,DAT_004869d4,&local_20,0x1000000,0);
  }
  else {
    local_c = (**(code **)(*local_8 + 0x1c))(local_8,0,0,DAT_004869d4,0,0x10);
  }
  if (DAT_0045dbd0 != 0) {
    FUN_0040e280(local_8);
  }
  return;
}

