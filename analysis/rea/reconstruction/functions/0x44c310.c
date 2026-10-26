// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x44c310
// name: FUN_0044c310
// ranges: 0x44c310-0x44c34f
// calls: 0x44c310, 0x44c350, 0x4510d0

int __cdecl FUN_0044c310(SIZE_T param_1,int param_2)

{
  int iVar1;
  
  if (0xffffffe0 < param_1) {
    return 0;
  }
  if (param_1 == 0) {
    param_1 = 1;
  }
  while( true ) {
    iVar1 = FUN_0044c350(param_1);
    if (iVar1 != 0) {
      return iVar1;
    }
    if (param_2 == 0) break;
    iVar1 = FUN_004510d0(param_1);
    if (iVar1 == 0) {
      return 0;
    }
  }
  return 0;
}
