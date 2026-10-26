// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x40b5c5
// name: FUN_0040b5c5
// ranges: 0x40b5c5-0x40b717
// calls: 0x40b5c5

undefined4 __fastcall FUN_0040b5c5(int param_1)

{
  int iVar1;
  double dVar2;
  LONG LVar3;
  float10 fVar4;
  float10 extraout_ST0;
  longlong lVar5;
  undefined1 local_34 [4];
  double local_30;
  double local_28;
  uint local_20;
  undefined8 local_1c;
  double local_14;
  double local_c;
  
  local_30 = (double)*(int *)(param_1 + 0x2c);
  local_c = (double)*(uint *)(param_1 + 0x14);
  local_1c = (double)*(uint *)(param_1 + 0x18);
  if (*(int *)(param_1 + 0x20) != 0) {
    while (LVar3 = InterlockedExchange((LONG *)(param_1 + 0x10),1), LVar3 != 0) {
      Sleep(2);
    }
    iVar1 = *(int *)(param_1 + 0xc);
    InterlockedExchange((LONG *)(param_1 + 0x10),0);
    if (iVar1 == 0) {
      return 0xffffffff;
    }
  }
  (**(code **)(**(int **)(param_1 + 4) + 0x10))(*(undefined4 *)(param_1 + 4),&local_20,local_34);
  dVar2 = local_30;
  if (*(uint *)(param_1 + 0x38) <= local_20) {
    dVar2 = local_30 - 1.0;
  }
  local_14 = dVar2 * local_c + (double)local_20;
  if (DAT_00462ae8 == 0) {
    fVar4 = (float10)local_14 / (float10)local_1c;
  }
  else {
    __adj_fdiv_m64((undefined4)local_1c,local_1c._4_4_);
    fVar4 = extraout_ST0;
  }
  local_28 = (double)fVar4;
  lVar5 = __ftol();
  return (int)lVar5;
}
