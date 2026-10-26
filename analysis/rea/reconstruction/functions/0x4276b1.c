// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4276b1
// name: FUN_004276b1
// ranges: 0x4276b1-0x4277ec
// calls: 0x4161f6, 0x4228b5, 0x4276b1

void __cdecl FUN_004276b1(int param_1)

{
  int *piVar1;
  int iVar2;
  int iVar3;
  HWND hWnd;
  float10 fVar4;
  float10 extraout_ST0;
  longlong lVar5;
  UINT Msg;
  WPARAM wParam;
  int local_c;
  
  piVar1 = *(int **)(*(int *)(param_1 + 0x10) + 8);
  iVar2 = *(int *)(param_1 + 8);
  local_c = -3000;
  if (DAT_00462ae8 == 0) {
    fVar4 = (float10)*(int *)(param_1 + 4) / (float10)50.0;
  }
  else {
    __adj_fdiv_m32(0x42480000);
    fVar4 = extraout_ST0;
  }
  if (DAT_00462ae8 != 0) {
    __adj_fdiv_m32((uint)(float)fVar4);
  }
  lVar5 = __ftol();
  while (local_c < iVar2 - (int)lVar5) {
    local_c = local_c + (int)lVar5;
    if (*(int *)(param_1 + 0xc) == 0) {
      (**(code **)(*piVar1 + 0x3c))(piVar1,local_c);
    }
    else {
      iVar3 = FUN_004228b5(local_c);
      if (iVar3 == -1) break;
    }
    Sleep(0x32);
  }
  if (*(int *)(param_1 + 0xc) == 0) {
    (**(code **)(*piVar1 + 0x3c))(piVar1,iVar2);
  }
  else {
    FUN_004228b5(iVar2);
  }
  wParam = 0xc353;
  Msg = 0x111;
  hWnd = (HWND)FUN_004161f6();
  PostMessageA(hWnd,Msg,wParam,param_1);
  return;
}
