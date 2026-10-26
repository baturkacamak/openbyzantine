// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x427460
// name: FUN_00427460
// ranges: 0x427460-0x4275cd
// calls: 0x4161f6, 0x422843, 0x4228b5, 0x422927, 0x427460, 0x44ac40

void __cdecl FUN_00427460(int param_1)

{
  int iVar1;
  HWND hWnd;
  float10 fVar2;
  float10 extraout_ST0;
  longlong lVar3;
  UINT Msg;
  WPARAM wParam;
  int *local_18;
  int local_8;
  
  if (DAT_00462ae8 == 0) {
    fVar2 = (float10)*(int *)(param_1 + 4) / (float10)50.0;
  }
  else {
    __adj_fdiv_m32(0x42480000);
    fVar2 = extraout_ST0;
  }
  if (*(int *)(param_1 + 0xc) == 0) {
    local_18 = *(int **)(*(int *)(param_1 + 0x10) + 8);
    (**(code **)(*local_18 + 0x18))(local_18,&local_8);
  }
  else {
    iVar1 = FUN_00422843(&local_8);
    if (iVar1 == -1) goto LAB_004275a0;
  }
  if (DAT_00462ae8 != 0) {
    __adj_fdiv_m32((uint)(float)fVar2);
  }
  lVar3 = __ftol();
  while (-3000 < local_8) {
    local_8 = local_8 - (int)lVar3;
    if (*(int *)(param_1 + 0xc) == 0) {
      (**(code **)(*local_18 + 0x3c))(local_18,local_8);
    }
    else {
      iVar1 = FUN_004228b5(local_8);
      if (iVar1 == -1) break;
    }
    Sleep(0x32);
  }
  if (*(int *)(param_1 + 0xc) == 0) {
    (**(code **)(*local_18 + 8))(local_18);
  }
  else {
    FUN_00422927();
  }
LAB_004275a0:
  FUN_0044ac40(*(LPVOID *)(param_1 + 0x10));
  wParam = 0xc354;
  Msg = 0x111;
  hWnd = (HWND)FUN_004161f6();
  PostMessageA(hWnd,Msg,wParam,param_1);
  return;
}
