// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x41809a
// name: FUN_0041809a
// ranges: 0x41809a-0x4180f3
// calls: 0x4161f6, 0x41809a

void __cdecl FUN_0041809a(LPRECT param_1)

{
  HWND hWnd;
  
  hWnd = (HWND)FUN_004161f6();
  GetClientRect(hWnd,param_1);
  ClientToScreen(hWnd,(LPPOINT)param_1);
  param_1->top = param_1->top + 0x14;
  param_1->bottom = param_1->top + 400;
  param_1->right = param_1->left + 0x280;
  return;
}
