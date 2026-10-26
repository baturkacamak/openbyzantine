// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x420587
// name: FUN_00420587
// ranges: 0x420587-0x420629
// calls: 0x41f80c, 0x420587

void __cdecl FUN_00420587(int param_1,int param_2)

{
  uint local_8;
  
  local_8 = (uint)(DAT_0046c5f7 == '\x01');
  if (param_1 == 0x10) {
    FUN_0041f80c(0x46c7f0,2,1,param_2);
  }
  else if (DAT_0045f7cc == 0) {
    FUN_0041f80c(0x46c7f0,0,1,param_2);
  }
  else {
    FUN_0041f80c(0x46c7f0,1,1,param_2);
  }
  FUN_0041f80c(0x46c7f0,3,local_8,param_2);
  return;
}
