// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x442760
// name: FUN_00442760
// ranges: 0x442760-0x44289c
// calls: 0x442760, 0x4428a0, 0x4429c0

void __cdecl FUN_00442760(int param_1,undefined4 param_2,uint *param_3,uint param_4)

{
  int iVar1;
  int iVar2;
  
  iVar1 = *(int *)(param_1 + 0x180);
  if (*(char *)(iVar1 + 0x30) == '\0') {
    iVar2 = (**(code **)(*(int *)(param_1 + 0x184) + 0xc))
                      (param_1,*(undefined4 *)(iVar1 + 0x38 + *(int *)(iVar1 + 0x40) * 4));
    if (iVar2 == 0) {
      return;
    }
    *(undefined1 *)(iVar1 + 0x30) = 1;
    *(int *)(iVar1 + 0x4c) = *(int *)(iVar1 + 0x4c) + 1;
  }
  iVar2 = *(int *)(iVar1 + 0x44);
  if (iVar2 != 0) {
    if (iVar2 == 1) goto LAB_00442838;
    if (iVar2 != 2) {
      return;
    }
    (**(code **)(*(int *)(param_1 + 0x188) + 4))
              (param_1,*(undefined4 *)(iVar1 + 0x38 + *(int *)(iVar1 + 0x40) * 4),iVar1 + 0x34,
               *(undefined4 *)(iVar1 + 0x48),param_2,param_3,param_4);
    if (*(uint *)(iVar1 + 0x34) < *(uint *)(iVar1 + 0x48)) {
      return;
    }
    *(undefined4 *)(iVar1 + 0x44) = 0;
    if (param_4 <= *param_3) {
      return;
    }
  }
  *(undefined4 *)(iVar1 + 0x34) = 0;
  *(int *)(iVar1 + 0x48) = *(int *)(param_1 + 0x114) + -1;
  if (*(int *)(param_1 + 0x118) == *(int *)(iVar1 + 0x4c)) {
    FUN_004429c0(param_1);
  }
  *(undefined4 *)(iVar1 + 0x44) = 1;
LAB_00442838:
  (**(code **)(*(int *)(param_1 + 0x188) + 4))
            (param_1,*(undefined4 *)(iVar1 + 0x38 + *(int *)(iVar1 + 0x40) * 4),iVar1 + 0x34,
             *(undefined4 *)(iVar1 + 0x48),param_2,param_3,param_4);
  if (*(uint *)(iVar1 + 0x48) <= *(uint *)(iVar1 + 0x34)) {
    if (*(int *)(iVar1 + 0x4c) == 1) {
      FUN_004428a0(param_1);
    }
    *(uint *)(iVar1 + 0x40) = *(uint *)(iVar1 + 0x40) ^ 1;
    *(undefined1 *)(iVar1 + 0x30) = 0;
    *(int *)(iVar1 + 0x34) = *(int *)(param_1 + 0x114) + 1;
    *(int *)(iVar1 + 0x48) = *(int *)(param_1 + 0x114) + 2;
    *(undefined4 *)(iVar1 + 0x44) = 2;
  }
  return;
}
