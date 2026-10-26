// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4280cc
// name: FUN_004280cc
// ranges: 0x4280cc-0x428142
// calls: 0x4280cc

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_004280cc(void)

{
  undefined4 uVar1;
  int local_14;
  int *local_10;
  int local_c [2];
  
  local_10 = DAT_00486218;
  if (DAT_00486218 == (int *)0x0) {
    uVar1 = 0;
  }
  else {
    (**(code **)(*DAT_00486218 + 0x10))(DAT_00486218,local_c,&local_14);
    if ((local_c[0] == 0) && (local_14 == 0)) {
      _DAT_00460190 = 0;
      uVar1 = 0;
    }
    else {
      _DAT_00460190 = 1;
      uVar1 = 1;
    }
  }
  return uVar1;
}
