// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x43ac17
// name: FUN_0043ac17
// ranges: 0x43ac17-0x43ad1d
// calls: 0x40b368, 0x439dbb, 0x43a03b, 0x43a20d, 0x43ac17, 0x43ad1e, 0x43adce, 0x43af73, 0x43d856, 0x44a820, 0x44ac40

undefined4 __fastcall FUN_0043ac17(undefined4 *param_1)

{
  int iVar1;
  undefined4 local_8;
  
  FUN_0040b368(param_1[1]);
  FUN_0043ad1e(param_1,param_1[0x20],&local_8);
  FUN_0043adce(param_1,0,local_8);
  iVar1 = FUN_0043af73(param_1,1);
  if (iVar1 == 0) {
    param_1[0x21] = param_1[0x21] + 1;
  }
  FUN_0043d856(param_1[3]);
  if (param_1[0x17] != 0) {
    FUN_0044a820((LPVOID)param_1[0x17]);
    param_1[0x17] = 0;
  }
  if (param_1[0x16] != 0) {
    FUN_0044ac40((LPVOID)param_1[0x16]);
    param_1[0x16] = 0;
  }
  FUN_0043a20d((int)param_1);
  FUN_0043a03b((int)param_1);
  FUN_00439dbb((int)param_1);
  PostMessageA((HWND)*param_1,0x111,0x9c44,param_1[0x21]);
  param_1[4] = 0;
  return 0;
}
