// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x43ed00
// name: FUN_0043ed00
// ranges: 0x43ed00-0x43ed9f
// calls: 0x43ed00

void __cdecl FUN_0043ed00(int *param_1)

{
  int iVar1;
  int iVar2;
  undefined4 *puVar3;
  int iVar4;
  undefined4 *puVar5;
  undefined4 *puVar6;
  int *local_8;
  int local_4;
  
  local_4 = 0;
  if (0 < param_1[0x48]) {
    local_8 = param_1 + 0x49;
    do {
      iVar1 = *local_8;
      if (*(int *)(iVar1 + 0x4c) == 0) {
        iVar2 = *(int *)(iVar1 + 0x10);
        if (((iVar2 < 0) || (3 < iVar2)) || (param_1[iVar2 + 0x24] == 0)) {
          *(undefined4 *)(*param_1 + 0x14) = 0x33;
          *(int *)(*param_1 + 0x18) = iVar2;
          (**(code **)*param_1)(param_1);
        }
        puVar3 = (undefined4 *)(**(code **)param_1[1])(param_1,1,0x82);
        puVar5 = (undefined4 *)param_1[iVar2 + 0x24];
        puVar6 = puVar3;
        for (iVar4 = 0x20; iVar4 != 0; iVar4 = iVar4 + -1) {
          *puVar6 = *puVar5;
          puVar5 = puVar5 + 1;
          puVar6 = puVar6 + 1;
        }
        *(undefined2 *)puVar6 = *(undefined2 *)puVar5;
        *(undefined4 **)(iVar1 + 0x4c) = puVar3;
      }
      local_8 = local_8 + 1;
      local_4 = local_4 + 1;
    } while (local_4 < param_1[0x48]);
  }
  return;
}
