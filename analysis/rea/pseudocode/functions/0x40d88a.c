// rea decompile_all
// name: FUN_0040d88a
// address: 0x40d88a
// body: available
// range: 0x40d88a-0x40d987 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


undefined4 FUN_0040d88a(void)

{
  int iVar1;
  int iVar2;
  HWND hWnd;
  undefined4 uVar3;
  tagPOINT local_c;
  
  GetCursorPos(&local_c);
  iVar2 = FUN_00416220();
  if (iVar2 == 0) {
    hWnd = (HWND)FUN_004161f6();
    ScreenToClient(hWnd,&local_c);
  }
  if ((local_c.x == DAT_00486a0a) && (local_c.y == DAT_00486a0c)) {
    uVar3 = 0;
  }
  else {
    DAT_00486a0a = (short)local_c.x;
    DAT_00486a0c = (short)local_c.y;
    if (DAT_00476338 < 0x20) {
      iVar2 = *(int *)(PTR_PTR_0045e534 + (uint)DAT_00476338 * 0x24 + 4);
      iVar1 = *(int *)(PTR_PTR_0045e534 + (uint)DAT_00476338 * 0x24 + 8);
    }
    else {
      iVar2 = *(int *)(PTR_PTR_0045e534 + 0x484);
      iVar1 = *(int *)(PTR_PTR_0045e534 + 0x488);
    }
    DAT_0046c44c = local_c.y - iVar1;
    DAT_0046c448 = local_c.x - iVar2;
    uVar3 = 1;
  }
  return uVar3;
}

