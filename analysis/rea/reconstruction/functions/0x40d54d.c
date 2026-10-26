// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x40d54d
// name: FUN_0040d54d
// ranges: 0x40d54d-0x40d5fe
// calls: 0x40d54d, 0x4164a0

void __cdecl
FUN_0040d54d(ushort *param_1,ushort *param_2,int param_3,int param_4,int param_5,int param_6)

{
  ushort uVar1;
  int local_18;
  int local_10;
  ushort *local_c;
  ushort *local_8;
  
  local_8 = param_2;
  if (param_6 == 0) {
    _memset(param_1,0,0x7d000);
  }
  else {
    local_c = param_1;
  }
  for (local_10 = 0; local_10 < param_4; local_10 = local_10 + 1) {
    for (local_18 = 0; local_18 < param_3; local_18 = local_18 + 1) {
      uVar1 = FUN_004164a0(*local_8);
      *local_c = uVar1;
      local_c = local_c + 1;
      local_8 = local_8 + 1;
    }
    local_c = local_c + (param_5 - param_3);
  }
  return;
}
