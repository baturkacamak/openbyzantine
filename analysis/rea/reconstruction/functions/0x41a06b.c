// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x41a06b
// name: FUN_0041a06b
// ranges: 0x41a06b-0x41a0c6
// calls: 0x40d9f7, 0x40da1f, 0x41a06b

void __cdecl FUN_0041a06b(undefined4 param_1)

{
  undefined4 local_14;
  undefined4 local_10;
  undefined4 local_c;
  undefined4 local_8;
  
  local_14 = 0;
  local_10 = 0;
  local_c = 0x280;
  local_8 = 0x1e0;
  DAT_0045f05c = 0;
  FUN_0040d9f7();
  (**(code **)(*DAT_004869d4 + 0x1c))(DAT_004869d4,0,0,param_1,&local_14,0x10);
  FUN_0040da1f();
  return;
}
