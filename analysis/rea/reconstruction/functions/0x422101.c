// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x422101
// name: FUN_00422101
// ranges: 0x422101-0x4221b0
// calls: 0x421ffc, 0x422101

void __cdecl FUN_00422101(int param_1,int param_2)

{
  int local_14;
  int local_10;
  int local_c;
  int local_8;
  
  local_10 = -1;
  local_8 = 0x4b;
  local_c = 0x73;
  if (param_1 == 0x24) {
    local_14 = 0x34;
    while( true ) {
      if ((0x3a < local_14) || (DAT_0045f7dc < local_14)) goto LAB_00422183;
      if ((local_8 <= param_2) && (param_2 <= local_c)) break;
      local_8 = local_8 + 0x28;
      local_c = local_c + 0x28;
      local_14 = local_14 + 1;
    }
    local_10 = local_14;
  }
LAB_00422183:
  if (local_10 != DAT_0045f7d8) {
    DAT_0045f7d8 = local_10;
    FUN_00421ffc(local_10,1);
  }
  return;
}
