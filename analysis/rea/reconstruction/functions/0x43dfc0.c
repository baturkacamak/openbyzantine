// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x43dfc0
// name: FUN_0043dfc0
// ranges: 0x43dfc0-0x43e032
// calls: 0x43dfc0, 0x43e040, 0x441a10

int __cdecl FUN_0043dfc0(int *param_1,char param_2)

{
  int iVar1;
  
  if ((param_1[4] != 200) && (param_1[4] != 0xc9)) {
    *(undefined4 *)(*param_1 + 0x14) = 0x12;
    *(int *)(*param_1 + 0x18) = param_1[4];
    (**(code **)*param_1)(param_1);
  }
  iVar1 = FUN_0043e040(param_1);
  if (iVar1 != 1) {
    if (iVar1 != 2) {
      return iVar1;
    }
    if (param_2 != '\0') {
      *(undefined4 *)(*param_1 + 0x14) = 0x32;
      (**(code **)*param_1)(param_1);
    }
    FUN_00441a10((int)param_1);
    return 2;
  }
  return 1;
}
