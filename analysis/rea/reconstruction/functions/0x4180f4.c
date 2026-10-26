// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4180f4
// name: FUN_004180f4
// ranges: 0x4180f4-0x4181c4
// calls: 0x416220, 0x4180f4

void __cdecl FUN_004180f4(int param_1)

{
  int iVar1;
  char local_408 [1024];
  int local_8;
  
  iVar1 = FUN_00416220();
  if (iVar1 != 0) {
    for (local_8 = 0; local_8 < 0x100; local_8 = local_8 + 1) {
      local_408[local_8 * 4] = *(char *)(local_8 * 3 + param_1) << 2;
      local_408[local_8 * 4 + 1] = *(char *)(local_8 * 3 + 1 + param_1) << 2;
      local_408[local_8 * 4 + 2] = *(char *)(local_8 * 3 + 2 + param_1) << 2;
    }
    (**(code **)(*DAT_004869d8 + 0x18))(DAT_004869d8,0,0,0x100,local_408);
    (**(code **)(*DAT_004869d4 + 0x7c))(DAT_004869d4,DAT_004869d8);
  }
  return;
}
