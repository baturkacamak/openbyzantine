// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x43e110
// name: FUN_0043e110
// ranges: 0x43e110-0x43e297
// calls: 0x43e110

void __cdecl FUN_0043e110(int *param_1)

{
  int iVar1;
  int *piVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  undefined4 uVar6;
  
  iVar1 = param_1[8];
  if (iVar1 == 1) {
    param_1[9] = 1;
    param_1[10] = 1;
    goto LAB_0043e24f;
  }
  if (iVar1 != 3) {
    if (iVar1 == 4) {
      if ((*(char *)((int)param_1 + 0x106) == '\0') || (*(char *)((int)param_1 + 0x107) == '\0')) {
        param_1[9] = 4;
      }
      else {
        if (*(char *)((int)param_1 + 0x107) != '\x02') {
          *(undefined4 *)(*param_1 + 0x14) = 0x6e;
          *(uint *)(*param_1 + 0x18) = (uint)*(byte *)((int)param_1 + 0x107);
          (**(code **)(*param_1 + 4))(param_1,0xffffffff);
        }
        param_1[9] = 5;
      }
      param_1[10] = 4;
    }
    else {
      param_1[9] = 0;
      param_1[10] = 0;
    }
    goto LAB_0043e24f;
  }
  if ((char)param_1[0x40] == '\0') {
    if (*(char *)((int)param_1 + 0x106) == '\0') {
      piVar2 = (int *)param_1[0x31];
      iVar1 = *piVar2;
      iVar3 = piVar2[0x15];
      iVar4 = piVar2[0x2a];
      if (((iVar1 == 1) && (iVar3 == 2)) && (iVar4 == 3)) goto LAB_0043e1ed;
      if (((iVar1 != 0x52) || (iVar3 != 0x47)) || (iVar4 != 0x42)) {
        iVar5 = *param_1;
        uVar6 = 1;
        *(int *)(iVar5 + 0x18) = iVar1;
        *(int *)(iVar5 + 0x1c) = iVar3;
        *(int *)(iVar5 + 0x20) = iVar4;
        *(undefined4 *)(*param_1 + 0x14) = 0x6b;
        goto LAB_0043e1e4;
      }
      param_1[9] = 2;
    }
    else {
      if (*(char *)((int)param_1 + 0x107) != '\0') {
        if (*(char *)((int)param_1 + 0x107) != '\x01') {
          *(undefined4 *)(*param_1 + 0x14) = 0x6e;
          uVar6 = 0xffffffff;
          *(uint *)(*param_1 + 0x18) = (uint)*(byte *)((int)param_1 + 0x107);
LAB_0043e1e4:
          (**(code **)(*param_1 + 4))(param_1,uVar6);
        }
        goto LAB_0043e1ed;
      }
      param_1[9] = 2;
    }
  }
  else {
LAB_0043e1ed:
    param_1[9] = 3;
  }
  param_1[10] = 2;
LAB_0043e24f:
  param_1[0xb] = 1;
  param_1[0xc] = 1;
  param_1[0xe] = 0;
  *(undefined1 *)(param_1 + 0x10) = 0;
  *(undefined1 *)((int)param_1 + 0x41) = 0;
  param_1[0x11] = 0;
  *(undefined1 *)(param_1 + 0x12) = 1;
  *(undefined1 *)((int)param_1 + 0x49) = 1;
  *(undefined1 *)((int)param_1 + 0x4a) = 0;
  *(undefined1 *)(param_1 + 0x14) = 1;
  param_1[0x1d] = 0;
  *(undefined1 *)(param_1 + 0x16) = 0;
  *(undefined1 *)((int)param_1 + 0x59) = 0;
  *(undefined1 *)((int)param_1 + 0x5a) = 0;
  param_1[0xf] = 0x3ff00000;
  param_1[0x13] = 2;
  param_1[0x15] = 0x100;
  return;
}
