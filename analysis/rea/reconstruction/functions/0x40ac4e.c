// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x40ac4e
// name: FUN_0040ac4e
// ranges: 0x40ac4e-0x40ac91
// calls: 0x40aa42, 0x40ac4e, 0x44ba08

void __cdecl FUN_0040ac4e(uint *param_1,int param_2)

{
  undefined1 local_68 [100];
  
  if (param_2 == 0xffff) {
    local_68[0] = 0;
  }
  else {
    FUN_0040aa42((uint *)local_68,param_2);
  }
  FUN_0044ba08(param_1,(uint *)local_68);
  return;
}
