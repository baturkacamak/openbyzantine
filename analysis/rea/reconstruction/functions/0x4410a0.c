// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4410a0
// name: FUN_004410a0
// ranges: 0x4410a0-0x44113e
// calls: 0x441080, 0x4410a0, 0x442300

undefined4 * __cdecl FUN_004410a0(int *param_1,int param_2,uint param_3)

{
  int *piVar1;
  undefined4 *puVar2;
  int iVar3;
  undefined4 *puVar4;
  
  iVar3 = param_1[1];
  if (0x3b9ac9f0 < param_3) {
    FUN_00441080(param_1,3);
  }
  if ((param_3 & 7) != 0) {
    param_3 = (param_3 - (param_3 & 7)) + 8;
  }
  if ((param_2 < 0) || (1 < param_2)) {
    *(undefined4 *)(*param_1 + 0x14) = 0xc;
    *(int *)(*param_1 + 0x18) = param_2;
    (**(code **)*param_1)(param_1);
  }
  puVar4 = (undefined4 *)FUN_00442300(param_1,param_3 + 0x10);
  if (puVar4 == (undefined4 *)0x0) {
    FUN_00441080(param_1,4);
  }
  piVar1 = (int *)(iVar3 + 0x48);
  *piVar1 = *piVar1 + param_3 + 0x10;
  puVar2 = (undefined4 *)(iVar3 + 0x38 + param_2 * 4);
  *puVar4 = *puVar2;
  puVar4[1] = param_3;
  puVar4[2] = 0;
  *puVar2 = puVar4;
  return puVar4 + 4;
}
