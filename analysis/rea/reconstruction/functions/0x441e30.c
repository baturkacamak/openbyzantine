// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x441e30
// name: FUN_00441e30
// ranges: 0x441e30-0x44201b
// calls: 0x441ab0, 0x441cf0, 0x441e30, 0x442020, 0x442370, 0x442a90, 0x443e70, 0x444530, 0x445360, 0x445600, 0x445930, 0x446080, 0x4465c0, 0x446bd0, 0x447f60

void __cdecl FUN_00441e30(int *param_1)

{
  int iVar1;
  char cVar2;
  int iVar3;
  
  iVar1 = param_1[0x5f];
  FUN_00441ab0(param_1);
  FUN_00442020((int)param_1);
  *(undefined4 *)(iVar1 + 0xc) = 0;
  iVar3 = FUN_00441cf0((int)param_1);
  *(char *)(iVar1 + 0x10) = (char)iVar3;
  *(undefined4 *)(iVar1 + 0x14) = 0;
  *(undefined4 *)(iVar1 + 0x18) = 0;
  if ((*(char *)((int)param_1 + 0x4a) == '\0') || ((char)param_1[0x10] == '\0')) {
    *(undefined1 *)(param_1 + 0x16) = 0;
    *(undefined1 *)((int)param_1 + 0x59) = 0;
    *(undefined1 *)((int)param_1 + 0x5a) = 0;
  }
  if (*(char *)((int)param_1 + 0x4a) != '\0') {
    if (*(char *)((int)param_1 + 0x41) != '\0') {
      *(undefined4 *)(*param_1 + 0x14) = 0x2e;
      (**(code **)*param_1)(param_1);
    }
    if (param_1[0x19] == 3) {
      if (param_1[0x1d] == 0) {
        if ((char)param_1[0x14] == '\0') {
          *(undefined1 *)(param_1 + 0x16) = 1;
        }
        else {
          *(undefined1 *)((int)param_1 + 0x5a) = 1;
        }
      }
      else {
        *(undefined1 *)((int)param_1 + 0x59) = 1;
      }
    }
    else {
      *(undefined1 *)(param_1 + 0x16) = 1;
      *(undefined1 *)((int)param_1 + 0x59) = 0;
      *(undefined1 *)((int)param_1 + 0x5a) = 0;
      param_1[0x1d] = 0;
    }
    if ((char)param_1[0x16] != '\0') {
      FUN_00447f60(param_1);
      *(int *)(iVar1 + 0x14) = param_1[0x69];
    }
    if ((*(char *)((int)param_1 + 0x5a) != '\0') || (*(char *)((int)param_1 + 0x59) != '\0')) {
      FUN_00446bd0(param_1);
      *(int *)(iVar1 + 0x18) = param_1[0x69];
    }
  }
  if (*(char *)((int)param_1 + 0x41) == '\0') {
    if (*(char *)(iVar1 + 0x10) == '\0') {
      FUN_00446080(param_1);
      FUN_00445930(param_1);
    }
    else {
      FUN_004465c0((int)param_1);
    }
    FUN_00445600((int)param_1,*(char *)((int)param_1 + 0x5a));
  }
  FUN_00445360((int)param_1);
  if (*(char *)((int)param_1 + 0xc9) == '\0') {
    if ((char)param_1[0x32] == '\0') {
      FUN_00443e70((int)param_1);
    }
    else {
      FUN_00444530((int)param_1);
    }
  }
  else {
    *(undefined4 *)(*param_1 + 0x14) = 1;
    (**(code **)*param_1)(param_1);
  }
  if ((*(char *)(param_1[99] + 0x10) != '\0') || (cVar2 = '\0', (char)param_1[0x10] != '\0')) {
    cVar2 = '\x01';
  }
  FUN_00442a90((int)param_1,cVar2);
  if (*(char *)((int)param_1 + 0x41) == '\0') {
    FUN_00442370(param_1,'\0');
  }
  (**(code **)(param_1[1] + 0x18))(param_1);
  (**(code **)(param_1[99] + 8))(param_1);
  if (((param_1[2] != 0) && ((char)param_1[0x10] == '\0')) &&
     (*(char *)(param_1[99] + 0x10) != '\0')) {
    iVar3 = param_1[8];
    if ((char)param_1[0x32] != '\0') {
      iVar3 = iVar3 * 3 + 2;
    }
    *(undefined4 *)(param_1[2] + 4) = 0;
    *(int *)(param_1[2] + 8) = param_1[0x46] * iVar3;
    *(undefined4 *)(param_1[2] + 0xc) = 0;
    *(uint *)(param_1[2] + 0x10) = 3 - (uint)(*(char *)((int)param_1 + 0x5a) == '\0');
    *(int *)(iVar1 + 0xc) = *(int *)(iVar1 + 0xc) + 1;
  }
  return;
}
