// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x41d6df
// name: FUN_0041d6df
// ranges: 0x41d6df-0x41d725; 0x41d72b-0x41d746; 0x41d797-0x41d7fe
// calls: 0x40d88a, 0x40d9f7, 0x40da1f, 0x4161f6, 0x41d523, 0x41d6df

INT_PTR __cdecl FUN_0041d6df(LPARAM param_1)

{
  HWND hWndParent;
  INT_PTR IVar1;
  bool bVar2;
  LPCSTR local_10;
  
  hWndParent = (HWND)FUN_004161f6();
  switch(param_1) {
  case 0:
  case 1:
  case 2:
  case 3:
  case 4:
  case 5:
  case 6:
    local_10 = (LPCSTR)0x72;
    break;
  case 7:
  case 0x3f:
    local_10 = (LPCSTR)0x73;
    break;
  default:
    local_10 = (LPCSTR)0x71;
  }
  bVar2 = DAT_0045dbd0 != 0;
  if (bVar2) {
    FUN_0040d9f7();
  }
  ShowCursor(1);
  IVar1 = DialogBoxParamA((HINSTANCE)0x0,local_10,hWndParent,FUN_0041d523,param_1);
  ShowCursor(0);
  if (bVar2) {
    FUN_0040d88a();
    FUN_0040da1f();
  }
  return IVar1;
}
