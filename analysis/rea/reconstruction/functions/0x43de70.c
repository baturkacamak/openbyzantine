// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x43de70
// name: FUN_0043de70
// ranges: 0x43de70-0x43df36
// calls: 0x43de70, 0x43ead0, 0x43f2f0, 0x440e20

void __cdecl FUN_0043de70(int *param_1,int param_2,int param_3)

{
  int iVar1;
  int iVar2;
  int *piVar3;
  
  param_1[1] = 0;
  if (param_2 != 0x3d) {
    *(undefined4 *)(*param_1 + 0x14) = 10;
    *(undefined4 *)(*param_1 + 0x18) = 0x3d;
    *(int *)(*param_1 + 0x1c) = param_2;
    (**(code **)*param_1)(param_1);
  }
  if (param_3 != 0x1a8) {
    *(undefined4 *)(*param_1 + 0x14) = 0x13;
    *(undefined4 *)(*param_1 + 0x18) = 0x1a8;
    *(int *)(*param_1 + 0x1c) = param_3;
    (**(code **)*param_1)(param_1);
  }
  iVar1 = *param_1;
  piVar3 = param_1;
  for (iVar2 = 0x6a; iVar2 != 0; iVar2 = iVar2 + -1) {
    *piVar3 = 0;
    piVar3 = piVar3 + 1;
  }
  *param_1 = iVar1;
  *(undefined1 *)(param_1 + 3) = 1;
  FUN_00440e20(param_1);
  param_1[2] = 0;
  param_1[5] = 0;
  param_1[0x24] = 0;
  param_1[0x25] = 0;
  param_1[0x26] = 0;
  param_1[0x27] = 0;
  iVar1 = 4;
  piVar3 = param_1 + 0x28;
  do {
    *piVar3 = 0;
    iVar1 = iVar1 + -1;
    piVar3[4] = 0;
    piVar3 = piVar3 + 1;
  } while (iVar1 != 0);
  FUN_0043f2f0((int)param_1);
  FUN_0043ead0((int)param_1);
  param_1[4] = 200;
  return;
}
