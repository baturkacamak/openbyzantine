// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x40d88a
// name: FUN_0040d88a
// ranges: 0x40d88a-0x40d987
// calls: 0x40d88a, 0x4161f6, 0x416220

undefined4 FUN_0040d88a(void)

{
  int iVar1;
  int iVar2;
  HWND hWnd;
  undefined4 uVar3;
  tagPOINT local_c;
  
  GetCursorPos(&local_c);
  iVar2 = FUN_00416220();
  if (iVar2 == 0) {
    hWnd = (HWND)FUN_004161f6();
    ScreenToClient(hWnd,&local_c);
  }
  if ((local_c.x == DAT_00486a0a) && (local_c.y == DAT_00486a0c)) {
    uVar3 = 0;
  }
  else {
    DAT_00486a0a = (short)local_c.x;
    DAT_00486a0c = (short)local_c.y;
    if (DAT_00476338 < 0x20) {
      iVar2 = *(int *)(PTR_PTR_0045e534 + (uint)DAT_00476338 * 0x24 + 4);
      iVar1 = *(int *)(PTR_PTR_0045e534 + (uint)DAT_00476338 * 0x24 + 8);
    }
    else {
      iVar2 = *(int *)(PTR_PTR_0045e534 + 0x484);
      iVar1 = *(int *)(PTR_PTR_0045e534 + 0x488);
    }
    DAT_0046c44c = local_c.y - iVar1;
    DAT_0046c448 = local_c.x - iVar2;
    uVar3 = 1;
  }
  return uVar3;
}
