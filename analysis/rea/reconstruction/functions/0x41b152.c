// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x41b152
// name: FUN_0041b152
// ranges: 0x41b152-0x41b250
// calls: 0x41b152

undefined4 __fastcall FUN_0041b152(int param_1)

{
  undefined4 uVar1;
  
  *(undefined4 *)(param_1 + 0x10) = 0;
  if (*(int *)(param_1 + 4) == 0) {
    uVar1 = 0xffffffff;
  }
  else {
    if (*(int *)(param_1 + 0x1d0) != 0) {
      SelectPalette(*(HDC *)(param_1 + 4),*(HPALETTE *)(param_1 + 0x1d0),0);
    }
    if (*(int *)(param_1 + 0x1d4) != 0) {
      SelectPalette(*(HDC *)(param_1 + 0x1c8),*(HPALETTE *)(param_1 + 0x1d4),0);
    }
    if (*(int *)(param_1 + 0x1b4) != 0) {
      DeleteObject(*(HGDIOBJ *)(param_1 + 0x1b4));
      *(undefined4 *)(param_1 + 0x1b4) = 0;
    }
    if (*(int *)(param_1 + 0x1cc) != 0) {
      SelectObject(*(HDC *)(param_1 + 0x1c8),*(HGDIOBJ *)(param_1 + 0x1cc));
      DeleteDC(*(HDC *)(param_1 + 0x1c8));
    }
    *(undefined4 *)(param_1 + 4) = 0;
    uVar1 = 0;
  }
  return uVar1;
}
