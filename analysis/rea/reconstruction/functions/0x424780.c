// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x424780
// name: FUN_00424780
// ranges: 0x424780-0x424980
// calls: 0x40701f, 0x40711b, 0x40ced0, 0x40d2a1, 0x41d440, 0x4212b4, 0x424780, 0x424b15, 0x424c4e, 0x424cb0, 0x44c630

void __cdecl FUN_00424780(int param_1,int param_2)

{
  int iVar1;
  float10 fVar2;
  float10 extraout_ST0;
  longlong lVar3;
  char local_20 [3];
  undefined1 local_1d;
  int local_18;
  uint *local_14;
  int local_10;
  int local_c;
  int local_8;
  
  local_c = 0;
  local_8 = 0;
  FUN_0044c630(local_20,(char *)(*(int *)(param_1 + 0x12dc + param_2 * 4) + 3),3);
  local_1d = 0;
  local_10 = FUN_0040ced0(local_20);
  iVar1 = DAT_0045fdbc;
  if ((local_10 != -1) && (iVar1 = local_10, DAT_0045fdbc == local_10)) {
    FUN_00424c4e(0);
    iVar1 = DAT_0045fdbc;
  }
  DAT_0045fdbc = iVar1;
  local_14 = *(uint **)(param_1 + 0x720 + param_2 * 4);
  iVar1 = __strcmpi((char *)local_14,&DAT_0045fdc0);
  if (iVar1 != 0) {
    local_c = 1;
    local_8 = 1;
  }
  iVar1 = _strcmp((char *)local_14,&DAT_0045fdc8);
  if (iVar1 == 0) {
    local_c = 0;
  }
  iVar1 = FUN_0041d440(6);
  if ((iVar1 == 1) && (local_c != 0)) {
    DAT_0045f054 = 1;
    if (DAT_0045f494 == 0) {
      local_18 = FUN_0040701f(local_14,0x212,4);
      FUN_00424b15(local_14);
    }
    else {
      local_18 = FUN_0040701f(local_14,0x1b8,0);
      FUN_00424cb0((char *)local_14,local_10);
    }
    if (DAT_00462ae8 == 0) {
      fVar2 = (float10)local_18 / (float10)DAT_0045fdb8;
    }
    else {
      __adj_fdiv_m32i();
      fVar2 = extraout_ST0;
    }
    _ceil((double)fVar2);
    lVar3 = __ftol();
    DAT_0045fdac = DAT_0045fdac + (int)lVar3;
    FUN_0040711b((char *)local_14);
  }
  if ((local_10 != -1) && (local_8 != 0)) {
    FUN_0040d2a1(*(uint **)(param_1 + 0x12dc + param_2 * 4),param_1 + 0x1e94);
  }
  iVar1 = FUN_0041d440(3);
  if ((iVar1 == 1) && (local_c != 0)) {
    FUN_004212b4(local_10,param_2);
  }
  return;
}
