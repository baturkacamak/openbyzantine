// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x40d5ff
// name: FUN_0040d5ff
// ranges: 0x40d5ff-0x40d6e4
// calls: 0x40d3af, 0x40d5ff, 0x41842b, 0x418bcf, 0x4196a4, 0x41d440, 0x42b280

undefined4 __cdecl FUN_0040d5ff(undefined4 param_1,int param_2,int param_3,int param_4)

{
  int iVar1;
  
  iVar1 = FUN_0041d440(1);
  if (iVar1 == 0) {
    if (DAT_0045dad8 != 0) {
      FUN_0041842b(DAT_0047a8a4,0x3e800);
    }
    FUN_0042b280(DAT_0046e82c,0x4b000);
  }
  if ((DAT_0046068c != 0) && (DAT_0046068c = 0, iVar1 == 0)) {
    FUN_004196a4(0,DAT_00477344,0);
  }
  FUN_0040d3af(param_1,param_2,param_3,param_4,1,0);
  FUN_00418bcf((int)DAT_0046e82c);
  DAT_0045dad8 = 0;
  return 1;
}
