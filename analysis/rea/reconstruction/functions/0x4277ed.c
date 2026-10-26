// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4277ed
// name: FUN_004277ed
// ranges: 0x4277ed-0x4278aa
// calls: 0x422a0d, 0x427460, 0x4277ed, 0x428143, 0x44c2f0

void __cdecl FUN_004277ed(int param_1)

{
  int iVar1;
  LPVOID lpParameter;
  HANDLE pvVar2;
  DWORD local_8;
  
  iVar1 = FUN_00428143(param_1);
  if (iVar1 == 0) {
    lpParameter = (LPVOID)FUN_0044c2f0(0x18);
    *(undefined4 *)((int)lpParameter + 4) =
         *(undefined4 *)(*(int *)(&DAT_00486220 + param_1 * 4) + 0x2c);
    *(undefined4 *)((int)lpParameter + 0x10) = *(undefined4 *)(&DAT_00485c90 + param_1 * 4);
    *(undefined4 *)(&DAT_00485c90 + param_1 * 4) = 0;
    iVar1 = FUN_00422a0d();
    if (iVar1 == param_1) {
      *(undefined4 *)((int)lpParameter + 0xc) = 1;
    }
    else {
      *(undefined4 *)((int)lpParameter + 0xc) = 0;
    }
    pvVar2 = CreateThread((LPSECURITY_ATTRIBUTES)0x0,0,FUN_00427460,lpParameter,0,&local_8);
    *(HANDLE *)((int)lpParameter + 0x14) = pvVar2;
    if (*(int *)((int)lpParameter + 0x14) != 0) {
      DAT_00460198 = DAT_00460198 + 1;
    }
  }
  return;
}
