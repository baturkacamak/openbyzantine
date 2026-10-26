// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x41516a
// name: FUN_0041516a
// ranges: 0x41516a-0x4151be
// calls: 0x41516a, 0x416220

undefined4 FUN_0041516a(void)

{
  int iVar1;
  
  iVar1 = FUN_00416220();
  if ((iVar1 != 0) &&
     (iVar1 = (**(code **)(*DAT_00486a04 + 0x50))(DAT_00486a04,DAT_0046c4e8,0x11), iVar1 != 0)) {
    return 1;
  }
  return 0;
}
