// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x40dd7f
// name: FUN_0040dd7f
// ranges: 0x40dd7f-0x40de7a
// calls: 0x40d88a, 0x40dae5, 0x40dd7f, 0x416220

undefined4 FUN_0040dd7f(void)

{
  int iVar1;
  uint local_7c;
  uint local_78;
  undefined4 local_74;
  undefined4 local_70;
  undefined4 local_6c;
  undefined4 local_68;
  undefined4 local_c;
  int local_8;
  
  _memset(&local_74,0,0x6c);
  local_74 = 0x6c;
  local_70 = 7;
  local_c = 0x840;
  local_68 = 0x43;
  local_6c = 0x36;
  local_8 = (**(code **)(*DAT_00486a04 + 0x18))(DAT_00486a04,&local_74,&DAT_0045dbd4,0);
  local_7c = (uint)DAT_00485c88;
  local_78 = (uint)DAT_00485c88;
  (**(code **)(*DAT_0045dbd4 + 0x74))(DAT_0045dbd4,8,&local_7c);
  iVar1 = (**(code **)(*DAT_00486a04 + 0x18))(DAT_00486a04,&local_74,&DAT_0045dbd8,0);
  local_8 = local_8 + iVar1;
  iVar1 = FUN_00416220();
  if (iVar1 == 0) {
    iVar1 = (**(code **)(*DAT_00486a04 + 0x18))(DAT_00486a04,&local_74,&DAT_0045dbdc,0);
    local_8 = local_8 + iVar1;
  }
  FUN_0040dae5(-1);
  ShowCursor(0);
  FUN_0040d88a();
  return 0;
}
