// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x414be7
// name: FUN_00414be7
// ranges: 0x414be7-0x414c3c
// calls: 0x414be7, 0x4161f6, 0x41d45a

void __cdecl FUN_00414be7(int param_1)

{
  HWND hWnd;
  uint *lpText;
  char *lpCaption;
  UINT uType;
  uint local_54 [20];
  
  DAT_0045f06c = 1;
  ReleaseCapture();
  ShowCursor(1);
  FUN_0041d45a(local_54,param_1);
  uType = 0x10000;
  lpCaption = s_Byzantine_0045f0ac;
  lpText = local_54;
  hWnd = (HWND)FUN_004161f6();
  MessageBoxA(hWnd,(LPCSTR)lpText,lpCaption,uType);
  return;
}
