// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x41590c
// name: FUN_0041590c
// ranges: 0x41590c-0x415a4d
// calls: 0x414cd2, 0x41590c

undefined4 __cdecl FUN_0041590c(HINSTANCE param_1)

{
  ATOM AVar1;
  undefined4 uVar2;
  int iVar3;
  int iVar4;
  HWND pHVar5;
  HMENU pHVar6;
  LPVOID pvVar7;
  WNDCLASSA local_30;
  uint local_8;
  
  local_30.style = 3;
  local_30.lpfnWndProc = FUN_00414cd2;
  local_30.cbClsExtra = 0;
  local_30.cbWndExtra = 0;
  local_30.hInstance = param_1;
  local_30.hIcon = (HICON)0x0;
  local_30.hCursor = LoadCursorA((HINSTANCE)0x0,(LPCSTR)0x7f00);
  local_30.hbrBackground = GetStockObject(4);
  local_30.lpszMenuName = s_Byzantine_0045f0d4;
  local_30.lpszClassName = s_Byzantine_0045f0e0;
  AVar1 = RegisterClassA(&local_30);
  local_8 = (uint)AVar1;
  if (local_8 == 0) {
    uVar2 = 0;
  }
  else {
    if (DAT_0045f080 == 0) {
      pvVar7 = (LPVOID)0x0;
      pHVar6 = (HMENU)0x0;
      pHVar5 = (HWND)0x0;
      iVar3 = GetSystemMetrics(1);
      iVar4 = GetSystemMetrics(0);
      DAT_0046c4e8 = CreateWindowExA(8,s_Byzantine_0045f108,s_Byzantine_0045f0fc,0x90000000,0,0,
                                     iVar4,iVar3,pHVar5,pHVar6,param_1,pvVar7);
    }
    else {
      pvVar7 = (LPVOID)0x0;
      pHVar6 = (HMENU)0x0;
      pHVar5 = (HWND)0x0;
      iVar3 = GetSystemMetrics(1);
      iVar4 = GetSystemMetrics(0);
      DAT_0046c4e8 = CreateWindowExA(8,s_Byzantine_0045f0f0,&DAT_0045f0ec,0x90000000,0,0,iVar4,iVar3
                                     ,pHVar5,pHVar6,param_1,pvVar7);
    }
    if (DAT_0046c4e8 == (HWND)0x0) {
      uVar2 = 0;
    }
    else {
      if (DAT_0045f080 == 0) {
        UpdateWindow(DAT_0046c4e8);
      }
      uVar2 = 1;
    }
  }
  return uVar2;
}
