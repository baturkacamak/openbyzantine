// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x40b445
// name: FUN_0040b445
// ranges: 0x40b445-0x40b5c4
// calls: 0x40b445

undefined4 __fastcall FUN_0040b445(int param_1)

{
  int iVar1;
  double dVar2;
  LONG LVar3;
  float10 fVar4;
  float10 extraout_ST0;
  longlong lVar5;
  undefined1 local_58 [4];
  double local_54;
  double local_4c;
  uint local_44 [10];
  undefined8 local_1c;
  double local_14;
  double local_c;
  
  local_54 = (double)*(int *)(param_1 + 0x2c);
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
  (**(code **)(**(int **)(param_1 + 4) + 0x10))(*(undefined4 *)(param_1 + 4),local_44,local_58);
  dVar2 = local_54;
  if (*(uint *)(param_1 + 0x38) <= local_44[0]) {
    dVar2 = local_54 - 1.0;
  }
  local_14 = dVar2 * local_c + (double)local_44[0];
  if (DAT_00462ae8 == 0) {
    fVar4 = (float10)local_14 / (float10)local_1c;
  }
  else {
    __adj_fdiv_m64((undefined4)local_1c,local_1c._4_4_);
    fVar4 = extraout_ST0;
  }
  local_4c = (double)fVar4 * 0.04535147547721863;
  lVar5 = __ftol();
  return (int)lVar5;
}
