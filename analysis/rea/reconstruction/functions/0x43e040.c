// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x43e040
// name: FUN_0043e040
// ranges: 0x43e040-0x43e0e4
// calls: 0x43e040, 0x43e110

int __cdecl FUN_0043e040(int *param_1)

{
  int iVar1;
  
  switch(param_1[4]) {
  case 200:
    (**(code **)(param_1[99] + 4))(param_1);
    (**(code **)(param_1[5] + 8))(param_1);
    param_1[4] = 0xc9;
switchD_0043e05d_caseD_c9:
    iVar1 = (**(code **)param_1[99])(param_1);
    if (iVar1 == 1) {
      FUN_0043e110(param_1);
      param_1[4] = 0xca;
    }
    return iVar1;
  case 0xc9:
    goto switchD_0043e05d_caseD_c9;
  case 0xca:
    return 1;
  case 0xcb:
  case 0xcc:
  case 0xcd:
  case 0xce:
  case 0xcf:
  case 0xd0:
  case 0xd2:
    iVar1 = (**(code **)param_1[99])(param_1);
    return iVar1;
  default:
    *(undefined4 *)(*param_1 + 0x14) = 0x12;
    *(int *)(*param_1 + 0x18) = param_1[4];
    (**(code **)*param_1)(param_1);
    return 0;
  }
}
