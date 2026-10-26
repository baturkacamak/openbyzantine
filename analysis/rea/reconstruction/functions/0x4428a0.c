// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4428a0
// name: FUN_004428a0
// ranges: 0x4428a0-0x4429be
// calls: 0x4428a0

void __cdecl FUN_004428a0(int param_1)

{
  int iVar1;
  int iVar2;
  undefined4 uVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  undefined4 *puVar7;
  undefined4 *puVar8;
  undefined4 *puVar9;
  undefined4 *puVar10;
  int local_24;
  int local_20;
  undefined4 *local_14;
  undefined4 *local_10;
  undefined4 *local_c;
  undefined4 *local_8;
  int local_4;
  
  iVar1 = *(int *)(param_1 + 0x180);
  iVar2 = *(int *)(param_1 + 0x114);
  local_4 = 0;
  local_20 = *(int *)(param_1 + 0xc4);
  if (0 < *(int *)(param_1 + 0x20)) {
    local_24 = 0;
    do {
      iVar4 = (*(int *)(local_20 + 0x24) * *(int *)(local_20 + 0xc)) / *(int *)(param_1 + 0x114);
      local_10 = *(undefined4 **)(*(int *)(iVar1 + 0x38) + local_24);
      puVar9 = *(undefined4 **)(*(int *)(iVar1 + 0x3c) + local_24);
      if (0 < iVar4) {
        iVar5 = (iVar2 + 2) * iVar4;
        iVar6 = (iVar2 + 1) * iVar4;
        local_8 = puVar9 + iVar6;
        puVar10 = local_10 + iVar6;
        puVar7 = local_10 + -iVar4;
        puVar8 = puVar9 + -iVar4;
        local_14 = puVar9 + iVar5;
        local_c = local_10 + iVar5;
        do {
          *puVar7 = *puVar10;
          uVar3 = *local_8;
          puVar10 = puVar10 + 1;
          puVar7 = puVar7 + 1;
          local_8 = local_8 + 1;
          *puVar8 = uVar3;
          *local_c = *local_10;
          iVar4 = iVar4 + -1;
          *local_14 = *puVar9;
          puVar8 = puVar8 + 1;
          puVar9 = puVar9 + 1;
          local_14 = local_14 + 1;
          local_10 = local_10 + 1;
          local_c = local_c + 1;
        } while (iVar4 != 0);
      }
      local_4 = local_4 + 1;
      local_24 = local_24 + 4;
      local_20 = local_20 + 0x54;
    } while (local_4 < *(int *)(param_1 + 0x20));
  }
  return;
}
