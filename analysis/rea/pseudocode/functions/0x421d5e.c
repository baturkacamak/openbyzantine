// rea decompile_all
// name: FUN_00421d5e
// address: 0x421d5e
// body: available
// range: 0x421d5e-0x421f3f inclusive
// range: 0x421f45-0x421f6f inclusive
// range: 0x421f75-0x421fa1 inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


undefined4 FUN_00421d5e(HWND param_1,int param_2,short param_3)

{
  HWND hWnd;
  HWND hWnd_00;
  int iVar1;
  uint *lpString;
  char *local_60;
  uint local_58 [20];
  HWND local_8;
  
  hWnd = GetDlgItem(param_1,0x3f7);
  if (param_2 == 0x110) {
    FUN_0041d45a(local_58,0x3c);
    lpString = local_58;
    hWnd_00 = GetDlgItem(param_1,0x3f9);
    SetWindowTextA(hWnd_00,(LPCSTR)lpString);
    if (DAT_0045f7d0 == -1) {
      SetWindowTextA(hWnd,&DAT_0045fcec);
    }
    else {
      SetWindowTextA(hWnd,&DAT_00484e30);
    }
    local_8 = GetDlgItem(param_1,1);
    if (local_8 != (HWND)0x0) {
      FUN_0041d45a(local_58,0x4e);
      SetWindowTextA(local_8,(LPCSTR)local_58);
    }
    local_8 = GetDlgItem(param_1,2);
    if (local_8 != (HWND)0x0) {
      FUN_0041d45a(local_58,0x4f);
      SetWindowTextA(local_8,(LPCSTR)local_58);
    }
    SetFocus(param_1);
  }
  else if (param_2 == 0x111) {
    if (param_3 == 1) {
      GetWindowTextA(hWnd,&DAT_00484e30,0x46);
      iVar1 = _strcmp(&DAT_00484e30,&DAT_0045fcf0);
      if (iVar1 == 0) {
        SetFocus(param_1);
      }
      else {
        EndDialog(param_1,1);
      }
      return 1;
    }
    if (param_3 == 2) {
      FUN_0044ba00((uint *)&DAT_00484e30,(uint *)&DAT_0045fcf4);
      EndDialog(param_1,0);
      return 1;
    }
    if (param_3 == 0x3f7) {
      GetWindowTextA(hWnd,(LPSTR)local_58,0x46);
      iVar1 = FUN_004061e7((int)local_58,2);
      if (iVar1 < 0xba) {
        local_60 = (char *)0x46;
      }
      else {
        local_60 = FUN_0044c580(local_58);
      }
      SendMessageA(hWnd,0xc5,(WPARAM)local_60,0);
      return 1;
    }
  }
  return 0;
}

