// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x420fe5
// name: FUN_00420fe5
// ranges: 0x420fe5-0x4210ae
// calls: 0x420dc9, 0x420f0a, 0x420fe5

void __cdecl FUN_00420fe5(int param_1,int param_2)

{
  int local_14;
  int local_10;
  int local_c;
  int local_8;
  
  local_10 = -1;
  local_8 = 0x6c;
  local_c = 0x80;
  if ((0x172 < param_1) && (param_1 < 0x230)) {
    for (local_14 = 0; local_14 < 0x10; local_14 = local_14 + 1) {
      if ((local_8 <= param_2) && (param_2 <= local_c)) {
        local_10 = *(int *)(&DAT_0046c800 + local_14 * 4);
        break;
      }
      local_c = local_8 + 0x28;
      local_8 = local_8 + 0x14;
    }
  }
  if (DAT_0045f7d4 != local_10) {
    DAT_0045f7d4 = local_10;
    FUN_00420dc9(local_10,1);
    FUN_00420f0a(local_10);
  }
  return;
}
