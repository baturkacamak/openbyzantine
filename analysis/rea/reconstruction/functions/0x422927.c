// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x422927
// name: FUN_00422927
// ranges: 0x422927-0x42295c
// calls: 0x4161f6, 0x422927

undefined4 FUN_00422927(void)

{
  undefined4 uVar1;
  HWND hWnd;
  UINT Msg;
  WPARAM wParam;
  LPARAM lParam;
  
  uVar1 = DAT_0045fd28;
  lParam = 0;
  wParam = 0x9c47;
  Msg = 0x111;
  hWnd = (HWND)FUN_004161f6();
  SendMessageA(hWnd,Msg,wParam,lParam);
  return uVar1;
}
