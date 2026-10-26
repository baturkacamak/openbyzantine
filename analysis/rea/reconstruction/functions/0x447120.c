// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x447120
// name: FUN_00447120
// ranges: 0x447120-0x4472e8
// calls: 0x447120

void __cdecl FUN_00447120(int param_1,int param_2,int param_3,int param_4,int param_5)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  uint uVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  int local_410;
  int aiStack_400 [256];
  
  iVar1 = *(int *)(param_1 + 0x70);
  iVar2 = param_2 + 0x18;
  iVar10 = param_3 + 0x1c;
  local_410 = 0x7fffffff;
  iVar3 = param_4 + 0x18;
  iVar4 = 0;
  if (0 < iVar1) {
    do {
      uVar6 = (uint)*(byte *)(**(int **)(param_1 + 0x74) + iVar4);
      if ((int)uVar6 < param_2) {
        iVar7 = uVar6 - iVar2;
        iVar5 = (uVar6 - param_2) * 2;
        iVar5 = iVar5 * iVar5;
      }
      else {
        if (iVar2 < (int)uVar6) {
          iVar5 = (uVar6 - iVar2) * 2;
          iVar5 = iVar5 * iVar5;
        }
        else {
          iVar5 = 0;
          if ((int)uVar6 <= iVar2 + param_2 >> 1) {
            iVar7 = uVar6 - iVar2;
            goto LAB_004471ee;
          }
        }
        iVar7 = uVar6 - param_2;
      }
LAB_004471ee:
      uVar6 = (uint)*(byte *)(*(int *)(*(int *)(param_1 + 0x74) + 4) + iVar4);
      if ((int)uVar6 < param_3) {
        iVar8 = uVar6 - iVar10;
        iVar9 = (uVar6 - param_3) * 3;
        iVar5 = iVar5 + iVar9 * iVar9;
      }
      else {
        if (iVar10 < (int)uVar6) {
          iVar8 = (uVar6 - iVar10) * 3;
          iVar5 = iVar5 + iVar8 * iVar8;
        }
        else if ((int)uVar6 <= param_3 + iVar10 >> 1) {
          iVar8 = uVar6 - iVar10;
          goto LAB_00447244;
        }
        iVar8 = uVar6 - param_3;
      }
LAB_00447244:
      uVar6 = (uint)*(byte *)(*(int *)(*(int *)(param_1 + 0x74) + 8) + iVar4);
      if ((int)uVar6 < param_4) {
        iVar9 = uVar6 - iVar3;
        iVar5 = iVar5 + (uVar6 - param_4) * (uVar6 - param_4);
      }
      else {
        if (iVar3 < (int)uVar6) {
          iVar5 = iVar5 + (uVar6 - iVar3) * (uVar6 - iVar3);
        }
        else if ((int)uVar6 <= param_4 + iVar3 >> 1) {
          iVar9 = uVar6 - iVar3;
          goto LAB_0044729a;
        }
        iVar9 = uVar6 - param_4;
      }
LAB_0044729a:
      iVar7 = iVar7 * 2 * iVar7 * 2 + iVar8 * 3 * iVar8 * 3 + iVar9 * iVar9;
      aiStack_400[iVar4] = iVar5;
      if (iVar7 < local_410) {
        local_410 = iVar7;
      }
      iVar4 = iVar4 + 1;
    } while (iVar4 < iVar1);
  }
  iVar2 = 0;
  iVar3 = 0;
  if (0 < iVar1) {
    do {
      if (aiStack_400[iVar3] <= local_410) {
        *(char *)(iVar2 + param_5) = (char)iVar3;
        iVar2 = iVar2 + 1;
      }
      iVar3 = iVar3 + 1;
    } while (iVar3 < iVar1);
  }
  return;
}
