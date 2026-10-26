// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4151bf
// name: FUN_004151bf
// ranges: 0x4151bf-0x4152dc
// calls: 0x4151bf

undefined4 __cdecl FUN_004151bf(int param_1)

{
  int iVar1;
  
  if (DAT_00486884 != (int *)0x0) {
    do {
      iVar1 = (**(code **)(*DAT_00486884 + 8))(DAT_00486884);
    } while (iVar1 != 0);
    DAT_00486884 = (int *)0x0;
  }
  if (DAT_004869d4 != (int *)0x0) {
    do {
      iVar1 = (**(code **)(*DAT_004869d4 + 8))(DAT_004869d4);
    } while (iVar1 != 0);
    DAT_004869d4 = (int *)0x0;
  }
  if (param_1 != 0) {
    iVar1 = (**(code **)(*DAT_00486a04 + 0x4c))(DAT_00486a04);
    if (iVar1 != 0) {
      return 0;
    }
    if (DAT_00486a04 != (int *)0x0) {
      do {
        iVar1 = (**(code **)(*DAT_00486a04 + 8))(DAT_00486a04);
      } while (iVar1 != 0);
      DAT_00486a04 = (int *)0x0;
    }
  }
  return 1;
}
