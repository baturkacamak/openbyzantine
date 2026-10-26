// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x421d5e
// name: FUN_00421d5e
// ranges: 0x421d5e-0x421f3f; 0x421f45-0x421f6f; 0x421f75-0x421fa1
// calls: 0x4061e7, 0x41d45a, 0x421d5e, 0x44ba00, 0x44c580

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
