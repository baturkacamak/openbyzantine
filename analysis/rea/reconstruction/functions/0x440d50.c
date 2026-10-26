// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x440d50
// name: FUN_00440d50
// ranges: 0x440d50-0x440ddc
// calls: 0x43f1d0, 0x440d50

uint __cdecl FUN_00440d50(int *param_1)

{
  uint uVar1;
  uint *puVar2;
  
  if (param_1[0x5e] == 0) {
    uVar1 = FUN_0043f1d0(param_1);
    if ((char)uVar1 == '\0') {
      return uVar1 & 0xffffff00;
    }
  }
  if (*(int *)(param_1[100] + 0x54) - param_1[0x5e] == -0xd0) {
    *(undefined4 *)(*param_1 + 0x14) = 0x61;
    *(undefined4 *)(*param_1 + 0x18) = *(undefined4 *)(param_1[100] + 0x54);
    (**(code **)(*param_1 + 4))(param_1,3);
    param_1[0x5e] = 0;
  }
  else {
    uVar1 = (**(code **)(param_1[5] + 0x14))(param_1,*(int *)(param_1[100] + 0x54));
    if ((char)uVar1 == '\0') {
      return uVar1 & 0xffffff00;
    }
  }
  puVar2 = (uint *)(param_1[100] + 0x54);
  *puVar2 = *(int *)(param_1[100] + 0x54) + 1U & 7;
  return CONCAT31((int3)((uint)puVar2 >> 8),1);
}
