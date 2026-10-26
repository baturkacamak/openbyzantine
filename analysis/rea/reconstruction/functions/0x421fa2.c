// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x421fa2
// name: FUN_00421fa2
// ranges: 0x421fa2-0x421ffb
// calls: 0x40d9f7, 0x40da1f, 0x4161f6, 0x421d5e, 0x421fa2

INT_PTR FUN_00421fa2(void)

{
  HWND hWndParent;
  INT_PTR IVar1;
  
  hWndParent = (HWND)FUN_004161f6();
  FUN_0040d9f7();
  ShowCursor(1);
  IVar1 = DialogBoxParamA((HINSTANCE)0x0,(LPCSTR)0x69,hWndParent,FUN_00421d5e,0);
  SetFocus(hWndParent);
  ShowCursor(0);
  FUN_0040da1f();
  return IVar1;
}
