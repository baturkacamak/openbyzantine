// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x418bcf
// name: FUN_00418bcf
// ranges: 0x418bcf-0x418c91
// calls: 0x40e280, 0x4161f6, 0x416220, 0x418040, 0x418bcf

void __cdecl FUN_00418bcf(int param_1)

{
  int iVar1;
  tagRECT local_20;
  HWND local_10;
  undefined4 local_c;
  int *local_8;
  
  local_8 = (int *)FUN_00418040(param_1);
  iVar1 = FUN_00416220();
  if (iVar1 == 0) {
    local_10 = (HWND)FUN_004161f6();
    GetClientRect(local_10,&local_20);
    ClientToScreen(local_10,(LPPOINT)&local_20);
    local_20.right = local_20.left + 0x280;
    local_20.bottom = local_20.top + 0x1e0;
    local_c = (**(code **)(*local_8 + 0x14))(local_8,0,DAT_004869d4,&local_20,0x1000000,0);
  }
  else {
    local_c = (**(code **)(*local_8 + 0x1c))(local_8,0,0,DAT_004869d4,0,0x10);
  }
  if (DAT_0045dbd0 != 0) {
    FUN_0040e280(local_8);
  }
  return;
}
