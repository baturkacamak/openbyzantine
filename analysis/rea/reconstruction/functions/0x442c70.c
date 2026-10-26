// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x442c70
// name: FUN_00442c70
// ranges: 0x442c70-0x442eb0
// calls: 0x4422c0, 0x442bc0, 0x442c70

undefined4 __cdecl FUN_00442c70(int param_1,int *param_2)

{
  int iVar1;
  int iVar2;
  int iVar3;
  code *pcVar4;
  int iVar5;
  char cVar6;
  uint uVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  int local_3c;
  uint local_38;
  int iStack_30;
  int iStack_24;
  undefined4 *puStack_20;
  int *piStack_1c;
  int *piStack_18;
  int iStack_14;
  int iStack_4;
  
  iVar1 = *(int *)(param_1 + 0x184);
  uVar7 = *(int *)(param_1 + 0x134) - 1;
  local_3c = *(int *)(iVar1 + 0x18);
  iVar2 = *(int *)(param_1 + 0x118);
  if (local_3c < *(int *)(iVar1 + 0x1c)) {
    do {
      local_38 = *(uint *)(iVar1 + 0x14);
      if (local_38 <= uVar7) {
        do {
          FUN_004422c0(*(undefined4 **)(iVar1 + 0x20),*(int *)(param_1 + 0x13c) << 7);
          cVar6 = (**(code **)(*(int *)(param_1 + 0x194) + 4))(param_1,(undefined4 *)(iVar1 + 0x20))
          ;
          if (cVar6 == '\0') {
            *(int *)(iVar1 + 0x18) = local_3c;
            *(uint *)(iVar1 + 0x14) = local_38;
            return 0;
          }
          iVar10 = 0;
          iStack_14 = 0;
          if (0 < *(int *)(param_1 + 0x120)) {
            piStack_18 = (int *)(param_1 + 0x124);
            piStack_1c = param_2;
            do {
              iVar3 = *piStack_18;
              if (*(char *)(iVar3 + 0x30) == '\0') {
                iVar10 = iVar10 + *(int *)(iVar3 + 0x3c);
              }
              else {
                pcVar4 = *(code **)(*(int *)(param_1 + 0x198) + 4 + *(int *)(iVar3 + 4) * 4);
                if (local_38 < uVar7) {
                  iVar8 = *(int *)(iVar3 + 0x34);
                }
                else {
                  iVar8 = *(int *)(iVar3 + 0x44);
                }
                iStack_30 = 0;
                iVar9 = *(int *)(iVar3 + 0x24) * local_3c * 4 + *piStack_1c;
                iVar5 = *(int *)(iVar3 + 0x40);
                if (0 < *(int *)(iVar3 + 0x38)) {
                  do {
                    if (((*(uint *)(param_1 + 0x80) < iVar2 - 1U) ||
                        (local_3c + iStack_30 < *(int *)(iVar3 + 0x48))) && (0 < iVar8)) {
                      puStack_20 = (undefined4 *)(iVar1 + 0x20 + iVar10 * 4);
                      iStack_24 = iVar5 * local_38;
                      iStack_4 = iVar8;
                      do {
                        (*pcVar4)(param_1,iVar3,*puStack_20,iVar9,iStack_24);
                        puStack_20 = puStack_20 + 1;
                        iStack_24 = iStack_24 + *(int *)(iVar3 + 0x24);
                        iStack_4 = iStack_4 + -1;
                      } while (iStack_4 != 0);
                    }
                    iVar10 = iVar10 + *(int *)(iVar3 + 0x34);
                    iStack_30 = iStack_30 + 1;
                    iVar9 = iVar9 + *(int *)(iVar3 + 0x24) * 4;
                  } while (iStack_30 < *(int *)(iVar3 + 0x38));
                }
              }
              piStack_1c = piStack_1c + 1;
              piStack_18 = piStack_18 + 1;
              iStack_14 = iStack_14 + 1;
            } while (iStack_14 < *(int *)(param_1 + 0x120));
          }
          local_38 = local_38 + 1;
        } while (local_38 <= uVar7);
      }
      local_3c = local_3c + 1;
      *(undefined4 *)(iVar1 + 0x14) = 0;
    } while (local_3c < *(int *)(iVar1 + 0x1c));
  }
  uVar7 = *(int *)(param_1 + 0x80) + 1;
  *(int *)(param_1 + 0x88) = *(int *)(param_1 + 0x88) + 1;
  *(uint *)(param_1 + 0x80) = uVar7;
  if (uVar7 < *(uint *)(param_1 + 0x118)) {
    FUN_00442bc0(param_1);
    return 3;
  }
  (**(code **)(*(int *)(param_1 + 0x18c) + 0xc))();
  return 4;
}
