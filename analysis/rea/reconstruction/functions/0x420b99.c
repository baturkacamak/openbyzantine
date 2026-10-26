// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x420b99
// name: FUN_00420b99
// ranges: 0x420b99-0x420c50
// calls: 0x41d7ff, 0x4201cc, 0x420b99, 0x4273f4

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int FUN_00420b99(void)

{
  int iVar1;
  undefined4 local_7c;
  undefined4 local_78;
  undefined4 local_74;
  undefined4 local_70;
  undefined4 local_14;
  int local_10;
  int local_c;
  
  local_c = 0;
  _memset(&local_7c,0,0x6c);
  local_7c = 0x6c;
  local_78 = 7;
  local_14 = 0x840;
  local_70 = 0xa8;
  local_74 = 0xa8;
  local_10 = (**(code **)(*DAT_00486a04 + 0x18))(DAT_00486a04,&local_7c,&DAT_0045f7c8,0);
  if (local_10 == 0) {
    iVar1 = FUN_004201cc();
    local_c = local_c + iVar1;
    iVar1 = FUN_0041d7ff();
    local_c = local_c + iVar1;
    DAT_0045f7dc = FUN_004273f4();
    _DAT_0045f794 = DAT_0045f78c + (DAT_0045f7dc + -0x33) * 0x28;
    local_10 = local_c;
  }
  return local_10;
}
