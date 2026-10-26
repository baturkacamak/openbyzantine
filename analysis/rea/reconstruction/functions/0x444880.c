// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x444880
// name: FUN_00444880
// ranges: 0x444880-0x444ac9
// calls: 0x443c50, 0x443d50, 0x444880, 0x444ad0

uint __cdecl FUN_00444880(int param_1,undefined4 *param_2)

{
  int iVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  int *piVar9;
  int *piVar10;
  int *local_48;
  undefined4 *local_44;
  int local_40;
  undefined4 local_38;
  undefined4 local_34;
  undefined4 local_30;
  uint local_2c;
  int local_28;
  int local_24;
  int local_20;
  undefined4 local_1c;
  short *local_18;
  int local_14 [5];
  
  iVar1 = *(int *)(param_1 + 0x194);
  local_1c = *(undefined4 *)(param_1 + 0x174);
  if (((*(int *)(param_1 + 0xfc) != 0) && (*(int *)(iVar1 + 0x28) == 0)) &&
     (uVar2 = FUN_00444ad0(param_1), (char)uVar2 == '\0')) {
    return uVar2 & 0xffffff00;
  }
  local_24 = param_1;
  local_38 = **(undefined4 **)(param_1 + 0x14);
  local_34 = (*(undefined4 **)(param_1 + 0x14))[1];
  local_20 = iVar1 + 0x10;
  local_30 = *(undefined4 *)(param_1 + 0x178);
  uVar2 = *(uint *)(iVar1 + 8);
  iVar7 = *(int *)(iVar1 + 0xc);
  piVar9 = (int *)(iVar1 + 0x14);
  piVar10 = local_14;
  for (iVar5 = 5; iVar5 != 0; iVar5 = iVar5 + -1) {
    *piVar10 = *piVar9;
    piVar9 = piVar9 + 1;
    piVar10 = piVar10 + 1;
  }
  local_40 = 0;
  if (0 < *(int *)(param_1 + 0x13c)) {
    local_48 = (int *)(param_1 + 0x140);
    local_44 = param_2;
    do {
      local_18 = (short *)*local_44;
      iVar5 = *local_48;
      iVar8 = *(int *)(iVar1 + 0x2c + *(int *)(*(int *)(param_1 + 0x124 + iVar5 * 4) + 0x14) * 4);
      if (iVar7 < 8) {
        uVar2 = FUN_00443c50(&local_38,uVar2,iVar7,0);
        if ((char)uVar2 == '\0') {
          return uVar2 & 0xffffff00;
        }
        uVar2 = local_2c;
        iVar7 = local_28;
        if (7 < local_28) goto LAB_0044498b;
        iVar6 = 1;
LAB_004449b5:
        uVar3 = FUN_00443d50(&local_38,uVar2,iVar7,iVar8,iVar6);
        uVar2 = local_2c;
        iVar7 = local_28;
        if ((int)uVar3 < 0) {
          return uVar3 & 0xffffff00;
        }
      }
      else {
LAB_0044498b:
        uVar3 = (int)uVar2 >> ((char)iVar7 - 8U & 0x1f) & 0xff;
        iVar6 = *(int *)(iVar8 + 0xd4 + uVar3 * 4);
        if (iVar6 == 0) {
          iVar6 = 9;
          goto LAB_004449b5;
        }
        uVar3 = (uint)*(byte *)(uVar3 + 0x4d4 + iVar8);
        iVar7 = iVar7 - iVar6;
      }
      uVar4 = 0;
      if (uVar3 != 0) {
        if ((iVar7 < (int)uVar3) &&
           (uVar4 = FUN_00443c50(&local_38,uVar2,iVar7,uVar3), uVar2 = local_2c, iVar7 = local_28,
           (char)uVar4 == '\0')) {
          return uVar4 & 0xffffff00;
        }
        iVar7 = iVar7 - uVar3;
        uVar4 = (1 << ((byte)uVar3 & 0x1f)) - 1U & (int)uVar2 >> ((byte)iVar7 & 0x1f);
        if ((int)uVar4 < *(int *)(&DAT_0045b578 + uVar3 * 4)) {
          uVar4 = *(int *)(&DAT_0045b5b8 + uVar3 * 4) + uVar4;
        }
      }
      local_40 = local_40 + 1;
      local_48 = local_48 + 1;
      iVar8 = uVar4 + local_14[iVar5 + 1];
      local_14[iVar5 + 1] = iVar8;
      local_44 = local_44 + 1;
      *local_18 = (short)iVar8 << ((byte)local_1c & 0x1f);
    } while (local_40 < *(int *)(param_1 + 0x13c));
  }
  **(undefined4 **)(param_1 + 0x14) = local_38;
  *(undefined4 *)(*(int *)(param_1 + 0x14) + 4) = local_34;
  *(undefined4 *)(param_1 + 0x178) = local_30;
  *(uint *)(iVar1 + 8) = uVar2;
  *(int *)(iVar1 + 0xc) = iVar7;
  piVar9 = local_14;
  piVar10 = (int *)(iVar1 + 0x14);
  for (iVar7 = 5; iVar7 != 0; iVar7 = iVar7 + -1) {
    *piVar10 = *piVar9;
    piVar9 = piVar9 + 1;
    piVar10 = piVar10 + 1;
  }
  *(int *)(iVar1 + 0x28) = *(int *)(iVar1 + 0x28) + -1;
  return CONCAT31((int3)(uVar2 >> 8),1);
}
