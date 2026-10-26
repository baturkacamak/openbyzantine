// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x43ee80
// name: FUN_0043ee80
// ranges: 0x43ee80-0x43f071
// calls: 0x43ee80, 0x442220

void __cdecl FUN_0043ee80(int *param_1)

{
  int iVar1;
  int iVar2;
  int iVar3;
  
  if ((0xffdc < param_1[7]) || (0xffdc < param_1[6])) {
    *(undefined4 *)(*param_1 + 0x14) = 0x28;
    *(undefined4 *)(*param_1 + 0x18) = 0xffdc;
    (**(code **)*param_1)(param_1);
  }
  if (param_1[0x30] != 8) {
    *(undefined4 *)(*param_1 + 0x14) = 0xd;
    *(int *)(*param_1 + 0x18) = param_1[0x30];
    (**(code **)*param_1)(param_1);
  }
  if (10 < param_1[8]) {
    *(undefined4 *)(*param_1 + 0x14) = 0x18;
    *(int *)(*param_1 + 0x18) = param_1[8];
    *(undefined4 *)(*param_1 + 0x1c) = 10;
    (**(code **)*param_1)(param_1);
  }
  iVar3 = 0;
  iVar2 = param_1[0x31];
  param_1[0x43] = 1;
  param_1[0x44] = 1;
  if (0 < param_1[8]) {
    do {
      if ((((*(int *)(iVar2 + 8) < 1) || (4 < *(int *)(iVar2 + 8))) || (*(int *)(iVar2 + 0xc) < 1))
         || (4 < *(int *)(iVar2 + 0xc))) {
        *(undefined4 *)(*param_1 + 0x14) = 0x10;
        (**(code **)*param_1)(param_1);
      }
      iVar1 = *(int *)(iVar2 + 8);
      if (*(int *)(iVar2 + 8) <= param_1[0x43]) {
        iVar1 = param_1[0x43];
      }
      param_1[0x43] = iVar1;
      iVar1 = *(int *)(iVar2 + 0xc);
      if (*(int *)(iVar2 + 0xc) <= param_1[0x44]) {
        iVar1 = param_1[0x44];
      }
      iVar3 = iVar3 + 1;
      iVar2 = iVar2 + 0x54;
      param_1[0x44] = iVar1;
    } while (iVar3 < param_1[8]);
  }
  iVar3 = 0;
  param_1[0x45] = 8;
  iVar2 = param_1[0x31];
  if (0 < param_1[8]) {
    do {
      *(undefined4 *)(iVar2 + 0x24) = 8;
      iVar3 = iVar3 + 1;
      iVar1 = FUN_00442220(*(int *)(iVar2 + 8) * param_1[6],param_1[0x43] << 3);
      *(int *)(iVar2 + 0x1c) = iVar1;
      iVar1 = FUN_00442220(*(int *)(iVar2 + 0xc) * param_1[7],param_1[0x44] << 3);
      *(int *)(iVar2 + 0x20) = iVar1;
      iVar1 = FUN_00442220(*(int *)(iVar2 + 8) * param_1[6],param_1[0x43]);
      *(int *)(iVar2 + 0x28) = iVar1;
      iVar1 = FUN_00442220(*(int *)(iVar2 + 0xc) * param_1[7],param_1[0x44]);
      *(int *)(iVar2 + 0x2c) = iVar1;
      *(undefined1 *)(iVar2 + 0x30) = 1;
      *(undefined4 *)(iVar2 + 0x4c) = 0;
      iVar2 = iVar2 + 0x54;
    } while (iVar3 < param_1[8]);
  }
  iVar2 = FUN_00442220(param_1[7],param_1[0x44] << 3);
  param_1[0x46] = iVar2;
  if ((param_1[8] <= param_1[0x48]) && ((char)param_1[0x32] == '\0')) {
    *(undefined1 *)(param_1[99] + 0x10) = 0;
    return;
  }
  *(undefined1 *)(param_1[99] + 0x10) = 1;
  return;
}
