// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x42a555
// name: FUN_0042a555
// ranges: 0x42a555-0x42a578
// calls: 0x42a555

int __cdecl FUN_0042a555(int param_1,int param_2)

{
  int iVar1;
  
  iVar1 = _rand();
  return param_1 + iVar1 % ((param_2 - param_1) + 1);
}
