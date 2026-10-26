// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x41cee8
// name: FUN_0041cee8
// ranges: 0x41cee8-0x41cf5e
// calls: 0x41cee8, 0x450fc0

int __cdecl FUN_0041cee8(int param_1)

{
  int iVar1;
  int iVar2;
  
  iVar1 = *(int *)(DAT_00483624 + param_1 * 4);
  iVar2 = *(int *)(DAT_00483624 + 4 + param_1 * 4) - iVar1;
  if (iVar2 == 0) {
    iVar2 = 0;
  }
  else if (DAT_004859d0 != 0xffffffff) {
    FUN_00450fc0(DAT_004859d0,iVar1,0);
  }
  return iVar2;
}
