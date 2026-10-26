// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4411f0
// name: FUN_004411f0
// ranges: 0x4411f0-0x4412a3
// calls: 0x440f60, 0x4410a0, 0x4411f0

int __cdecl FUN_004411f0(int *param_1,int param_2,int param_3,uint param_4)

{
  uint uVar1;
  int iVar2;
  uint uVar3;
  undefined4 *puVar4;
  undefined4 *puVar5;
  uint uVar6;
  
  iVar2 = param_1[1];
  uVar1 = (uint)(0x3b9ac9f0 / (ulonglong)(uint)(param_3 * 0x80));
  if (uVar1 == 0) {
    *(undefined4 *)(*param_1 + 0x14) = 0x45;
    (**(code **)*param_1)(param_1);
  }
  if ((int)param_4 <= (int)uVar1) {
    uVar1 = param_4;
  }
  *(uint *)(iVar2 + 0x4c) = uVar1;
  uVar6 = 0;
  iVar2 = FUN_00440f60(param_1,param_2,param_4 << 2);
  if (param_4 != 0) {
    do {
      uVar3 = param_4 - uVar6;
      if (uVar1 <= param_4 - uVar6) {
        uVar3 = uVar1;
      }
      puVar4 = FUN_004410a0(param_1,param_2,uVar3 * param_3 * 0x80);
      if (uVar3 != 0) {
        puVar5 = (undefined4 *)(iVar2 + uVar6 * 4);
        uVar6 = uVar6 + uVar3;
        uVar1 = uVar3;
        do {
          *puVar5 = puVar4;
          puVar5 = puVar5 + 1;
          puVar4 = puVar4 + param_3 * 0x20;
          uVar1 = uVar1 - 1;
        } while (uVar1 != 0);
      }
      uVar1 = uVar3;
    } while (uVar6 < param_4);
  }
  return iVar2;
}
