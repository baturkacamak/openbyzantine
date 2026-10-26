// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x440f60
// name: FUN_00440f60
// ranges: 0x440f60-0x44107e
// calls: 0x440f60, 0x441080, 0x4422e0

int __cdecl FUN_00440f60(int *param_1,int param_2,uint param_3)

{
  int iVar1;
  int *piVar2;
  int iVar3;
  undefined4 *puVar4;
  int iVar5;
  undefined4 *puVar6;
  undefined4 *puVar7;
  uint uVar8;
  undefined4 *local_c;
  
  iVar5 = param_1[1];
  if (0x3b9ac9f0 < param_3) {
    FUN_00441080(param_1,1);
  }
  if ((param_3 & 7) != 0) {
    param_3 = (param_3 - (param_3 & 7)) + 8;
  }
  if ((param_2 < 0) || (1 < param_2)) {
    *(undefined4 *)(*param_1 + 0x14) = 0xc;
    *(int *)(*param_1 + 0x18) = param_2;
    (**(code **)*param_1)(param_1);
  }
  iVar3 = param_2 * 4;
  local_c = (undefined4 *)0x0;
  puVar4 = (undefined4 *)(iVar3 + 0x30 + iVar5);
  puVar7 = (undefined4 *)*puVar4;
  if (puVar7 != (undefined4 *)0x0) {
    do {
      puVar6 = puVar7;
      puVar7 = puVar6;
      if (param_3 <= (uint)puVar6[2]) break;
      puVar7 = (undefined4 *)*puVar6;
      local_c = puVar6;
    } while (puVar7 != (undefined4 *)0x0);
    if (puVar7 != (undefined4 *)0x0) goto LAB_00441066;
  }
  iVar1 = param_3 + 0x10;
  if (local_c == (undefined4 *)0x0) {
    uVar8 = *(uint *)(&DAT_0045b3a8 + iVar3);
  }
  else {
    uVar8 = *(uint *)(&DAT_0045b3b0 + iVar3);
  }
  if (1000000000U - iVar1 < uVar8) {
    uVar8 = 1000000000U - iVar1;
  }
  while (puVar7 = (undefined4 *)FUN_004422e0(param_1,uVar8 + iVar1), puVar7 == (undefined4 *)0x0) {
    uVar8 = uVar8 >> 1;
    if (uVar8 < 0x32) {
      FUN_00441080(param_1,2);
    }
  }
  piVar2 = (int *)(iVar5 + 0x48);
  *piVar2 = *piVar2 + uVar8 + iVar1;
  *puVar7 = 0;
  puVar7[1] = 0;
  puVar7[2] = uVar8 + param_3;
  if (local_c == (undefined4 *)0x0) {
    local_c = puVar4;
  }
  *local_c = puVar7;
LAB_00441066:
  iVar5 = puVar7[1];
  puVar7[1] = iVar5 + param_3;
  puVar7[2] = puVar7[2] - param_3;
  return iVar5 + 0x10 + (int)puVar7;
}
