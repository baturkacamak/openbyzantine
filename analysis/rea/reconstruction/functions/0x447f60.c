// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x447f60
// name: FUN_00447f60
// ranges: 0x447f60-0x447ff6
// calls: 0x447f60, 0x448000, 0x448270, 0x4483f0

void __cdecl FUN_00447f60(int *param_1)

{
  undefined4 *puVar1;
  
  puVar1 = (undefined4 *)(**(code **)param_1[1])(param_1,1,0x58);
  param_1[0x69] = (int)puVar1;
  *puVar1 = &LAB_00448430;
  puVar1[2] = &DAT_00448b80;
  puVar1[3] = &LAB_00448b90;
  puVar1[0x11] = 0;
  puVar1[0xd] = 0;
  if (4 < param_1[0x19]) {
    *(undefined4 *)(*param_1 + 0x14) = 0x36;
    *(undefined4 *)(*param_1 + 0x18) = 4;
    (**(code **)*param_1)(param_1);
  }
  if (0x100 < param_1[0x15]) {
    *(undefined4 *)(*param_1 + 0x14) = 0x38;
    *(undefined4 *)(*param_1 + 0x18) = 0x100;
    (**(code **)*param_1)(param_1);
  }
  FUN_00448000(param_1);
  FUN_00448270((int)param_1);
  if (param_1[0x13] == 2) {
    FUN_004483f0((int)param_1);
  }
  return;
}
