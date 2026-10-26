// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4288e5
// name: FUN_004288e5
// ranges: 0x4288e5-0x428a15
// calls: 0x41d440, 0x422a90, 0x427de1, 0x42826e, 0x4288e5

void FUN_004288e5(void)

{
  int iVar1;
  int iVar2;
  int local_14;
  uint *local_8;
  
  iVar1 = FUN_00422a90();
  for (local_8 = DAT_0046019c; local_8 != (uint *)0x0; local_8 = (uint *)local_8[0x16]) {
    if (local_8[0x15] == 0) {
      FUN_0042826e(local_8);
    }
    else if ((local_8[0xc] != 0) || (iVar2 = FUN_0041d440(2), iVar2 == 1)) {
      (**(code **)(**(int **)(local_8[0x15] + 8) + 0x30))(*(int **)(local_8[0x15] + 8),0,0,1);
    }
  }
  for (local_14 = 0; local_14 < 0x15e; local_14 = local_14 + 1) {
    if ((*(int *)(&DAT_00486220 + local_14 * 4) != 0) &&
       (*(int *)(*(int *)(&DAT_00486220 + local_14 * 4) + 0x34) != 0)) {
      if ((*(char *)(*(int *)(&DAT_0047a8e0 + local_14 * 4) + 8) != '\x02') || (iVar1 == -1)) {
        FUN_00427de1(local_14,0);
      }
      if (*(int *)(*(int *)(&DAT_00486220 + local_14 * 4) + 0x14) == 0) {
        *(undefined4 *)(*(int *)(&DAT_00486220 + local_14 * 4) + 0x34) = 0;
      }
    }
  }
  return;
}
