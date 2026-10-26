// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x412cc4
// name: FUN_00412cc4
// ranges: 0x412cc4-0x412d34
// calls: 0x412cc4, 0x44c2f0

void __cdecl FUN_00412cc4(int *param_1)

{
  int iVar1;
  
  iVar1 = FUN_0044c2f0(0x14);
  *param_1 = iVar1;
  *(undefined2 *)*param_1 = 0xffff;
  *(undefined2 *)(*param_1 + 2) = 0xffff;
  *(undefined4 *)(*param_1 + 4) = 0;
  *(undefined4 *)(*param_1 + 8) = 0;
  *(undefined4 *)(*param_1 + 0xc) = 0;
  *(undefined1 *)(*param_1 + 0x10) = 1;
  *(undefined1 *)(*param_1 + 0x11) = 0;
  return;
}
