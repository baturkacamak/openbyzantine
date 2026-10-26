// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x41d523
// name: FUN_0041d523
// ranges: 0x41d523-0x41d670; 0x41d676-0x41d694; 0x41d6a8-0x41d6ac; 0x41d6b2-0x41d6de
// calls: 0x41d45a, 0x41d523

undefined4 FUN_0041d523(HWND param_1,int param_2,ushort param_3,int param_4)

{
  HWND hWnd;
  uint *lpString;
  uint local_58 [20];
  HWND local_8;
  
  if (param_2 == 0x110) {
    FUN_0041d45a(local_58,param_4);
    lpString = local_58;
    hWnd = GetDlgItem(param_1,0x3f9);
    SetWindowTextA(hWnd,(LPCSTR)lpString);
    local_8 = GetDlgItem(param_1,6);
    if (local_8 != (HWND)0x0) {
      FUN_0041d45a(local_58,0x4c);
      SetWindowTextA(local_8,(LPCSTR)local_58);
    }
    local_8 = GetDlgItem(param_1,7);
    if (local_8 != (HWND)0x0) {
      FUN_0041d45a(local_58,0x4d);
      SetWindowTextA(local_8,(LPCSTR)local_58);
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
    switch(param_3) {
    case 1:
    case 2:
    case 6:
    case 7:
      EndDialog(param_1,(uint)param_3);
      return 1;
    default:
    }
  }
  return 0;
}
