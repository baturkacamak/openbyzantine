// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x43e560
// name: FUN_0043e560
// ranges: 0x43e560-0x43e5fa
// calls: 0x43e560

int __cdecl FUN_0043e560(int *param_1,undefined4 param_2,undefined4 param_3)

{
  int iStack_4;
  
  if (param_1[4] != 0xcd) {
    *(undefined4 *)(*param_1 + 0x14) = 0x12;
    *(int *)(*param_1 + 0x18) = param_1[4];
    (**(code **)*param_1)(param_1);
  }
  if ((uint)param_1[0x18] <= (uint)param_1[0x1e]) {
    *(undefined4 *)(*param_1 + 0x14) = 0x77;
    (**(code **)(*param_1 + 4))(param_1,0xffffffff);
    return 0;
  }
  if (param_1[2] != 0) {
    *(int *)(param_1[2] + 4) = param_1[0x1e];
    *(int *)(param_1[2] + 8) = param_1[0x18];
    (**(code **)param_1[2])(param_1);
  }
  iStack_4 = 0;
  (**(code **)(param_1[0x60] + 4))(param_1,param_2,&iStack_4,param_3);
  param_1[0x1e] = param_1[0x1e] + iStack_4;
  return iStack_4;
}
