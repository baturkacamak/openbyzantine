// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4445a0
// name: FUN_004445a0
// ranges: 0x4445a0-0x44487b
// calls: 0x443a70, 0x4445a0, 0x444880, 0x444b50, 0x444df0

void __cdecl FUN_004445a0(int *param_1)

{
  int *piVar1;
  int iVar2;
  int iVar3;
  bool bVar4;
  int iVar5;
  undefined4 *puVar6;
  int iVar7;
  int *piVar8;
  bool bVar9;
  int *local_8;
  int local_4;
  
  iVar2 = param_1[0x65];
  bVar9 = param_1[0x5a] != 0;
  bVar4 = false;
  if (bVar9) {
    if ((param_1[0x5b] < param_1[0x5a]) || (0x3f < param_1[0x5b])) {
      bVar4 = true;
    }
    if (param_1[0x48] == 1) goto LAB_004445f3;
  }
  else if (param_1[0x5b] == 0) goto LAB_004445f3;
  bVar4 = true;
LAB_004445f3:
  if ((param_1[0x5c] != 0) && (param_1[0x5c] - param_1[0x5d] != 1)) {
    bVar4 = true;
  }
  if (0xd < param_1[0x5d]) {
    bVar4 = true;
  }
  if (bVar4) {
    *(undefined4 *)(*param_1 + 0x14) = 0xe;
    *(int *)(*param_1 + 0x18) = param_1[0x5a];
    *(int *)(*param_1 + 0x1c) = param_1[0x5b];
    *(int *)(*param_1 + 0x20) = param_1[0x5c];
    *(int *)(*param_1 + 0x24) = param_1[0x5d];
    (**(code **)*param_1)(param_1);
  }
  local_4 = 0;
  if (0 < param_1[0x48]) {
    local_8 = param_1 + 0x49;
    do {
      iVar3 = *(int *)(*local_8 + 4);
      piVar8 = (int *)(iVar3 * 0x100 + param_1[0x23]);
      if ((bVar9) && (*piVar8 < 0)) {
        *(undefined4 *)(*param_1 + 0x14) = 0x6f;
        *(int *)(*param_1 + 0x18) = iVar3;
        *(undefined4 *)(*param_1 + 0x1c) = 0;
        (**(code **)(*param_1 + 4))(param_1,0xffffffff);
      }
      iVar7 = param_1[0x5a];
      if (iVar7 <= param_1[0x5b]) {
        do {
          iVar5 = piVar8[iVar7];
          if (iVar5 < 1) {
            iVar5 = 0;
          }
          if (iVar5 != param_1[0x5c]) {
            *(undefined4 *)(*param_1 + 0x14) = 0x6f;
            *(int *)(*param_1 + 0x18) = iVar3;
            *(int *)(*param_1 + 0x1c) = iVar7;
            (**(code **)(*param_1 + 4))(param_1,0xffffffff);
          }
          piVar8[iVar7] = param_1[0x5d];
          iVar7 = iVar7 + 1;
        } while (iVar7 <= param_1[0x5b]);
      }
      local_8 = local_8 + 1;
      local_4 = local_4 + 1;
    } while (local_4 < param_1[0x48]);
  }
  if (param_1[0x5c] == 0) {
    if (bVar9) {
      *(code **)(iVar2 + 4) = FUN_00444b50;
    }
    else {
      *(code **)(iVar2 + 4) = FUN_00444880;
    }
  }
  else if (bVar9) {
    *(undefined1 **)(iVar2 + 4) = &LAB_00444f10;
  }
  else {
    *(code **)(iVar2 + 4) = FUN_00444df0;
  }
  local_4 = 0;
  if (0 < param_1[0x48]) {
    piVar8 = param_1 + 0x49;
    puVar6 = (undefined4 *)(iVar2 + 0x18);
    do {
      if (bVar9) {
        iVar3 = *(int *)(*piVar8 + 0x18);
        if (((iVar3 < 0) || (3 < iVar3)) || (param_1[iVar3 + 0x2c] == 0)) {
          *(undefined4 *)(*param_1 + 0x14) = 0x31;
          *(int *)(*param_1 + 0x18) = iVar3;
          (**(code **)*param_1)(param_1);
        }
        piVar1 = (int *)(iVar2 + 0x2c + iVar3 * 4);
        FUN_00443a70((int)param_1,param_1[iVar3 + 0x2c],piVar1);
        *(int *)(iVar2 + 0x3c) = *piVar1;
      }
      else if (param_1[0x5c] == 0) {
        iVar3 = *(int *)(*piVar8 + 0x14);
        if (((iVar3 < 0) || (3 < iVar3)) || (param_1[iVar3 + 0x28] == 0)) {
          *(undefined4 *)(*param_1 + 0x14) = 0x31;
          *(int *)(*param_1 + 0x18) = iVar3;
          (**(code **)*param_1)(param_1);
        }
        FUN_00443a70((int)param_1,param_1[iVar3 + 0x28],(int *)(iVar2 + iVar3 * 4 + 0x2c));
      }
      *puVar6 = 0;
      puVar6 = puVar6 + 1;
      piVar8 = piVar8 + 1;
      local_4 = local_4 + 1;
    } while (local_4 < param_1[0x48]);
  }
  *(undefined4 *)(iVar2 + 0xc) = 0;
  *(undefined4 *)(iVar2 + 8) = 0;
  *(undefined1 *)(iVar2 + 0x10) = 0;
  *(undefined4 *)(iVar2 + 0x14) = 0;
  *(int *)(iVar2 + 0x28) = param_1[0x3f];
  return;
}
