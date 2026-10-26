// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x44cc60
// name: FUN_0044cc60
// ranges: 0x44cc60-0x44cc81
// calls: 0x44cc60

undefined4 __cdecl FUN_0044cc60(HANDLE param_1)

{
  BOOL BVar1;
  
  BVar1 = FindClose(param_1);
  if (BVar1 == 0) {
    DAT_00462c74 = 0x16;
    return 0xffffffff;
  }
  return 0;
}
