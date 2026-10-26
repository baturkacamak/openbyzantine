// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4275ce
// name: FUN_004275ce
// ranges: 0x4275ce-0x4276b0
// calls: 0x4275ce, 0x4276b1, 0x44c2f0

void __cdecl FUN_004275ce(int param_1,int param_2,int param_3)

{
  int *lpParameter;
  HANDLE pvVar1;
  DWORD local_8;
  
  if (param_2 == 0) {
    if (*(int *)(&DAT_00485c90 + param_1 * 4) == 0) {
      return;
    }
    param_2 = *(int *)(&DAT_00485c90 + param_1 * 4);
  }
  if (*(int *)(&DAT_00486220 + param_1 * 4) != 0) {
    lpParameter = (int *)FUN_0044c2f0(0x18);
    lpParameter[4] = param_2;
    *(undefined4 *)(&DAT_00485c90 + param_1 * 4) = 0;
    *lpParameter = param_1;
    lpParameter[1] = *(int *)(*(int *)(&DAT_00486220 + param_1 * 4) + 0x28);
    lpParameter[2] = *(int *)(*(int *)(&DAT_00486220 + param_1 * 4) + 0x1c) + -10000;
    lpParameter[3] = param_3;
    pvVar1 = CreateThread((LPSECURITY_ATTRIBUTES)0x0,0,FUN_004276b1,lpParameter,0,&local_8);
    lpParameter[5] = (int)pvVar1;
    if (lpParameter[5] != 0) {
      DAT_00460198 = DAT_00460198 + 1;
    }
  }
  return;
}
