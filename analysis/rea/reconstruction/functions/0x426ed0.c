// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x426ed0
// name: FUN_00426ed0
// ranges: 0x426ed0-0x426f94
// calls: 0x40ac38, 0x40ac43, 0x426ed0

undefined4 __cdecl FUN_00426ed0(int param_1,int param_2)

{
  undefined4 local_c;
  undefined4 local_8;
  
  local_8 = 0;
  if (param_2 == 0xffff) {
    *(undefined4 *)(param_1 + 0x20) = 0;
  }
  else {
    *(undefined4 *)(param_1 + 0x20) = *(undefined4 *)(param_1 + 0x18);
    for (local_c = 0; local_c < param_2; local_c = local_c + 1) {
      if (*(int *)(*(int *)(param_1 + 0x20) + 4) == 0) {
        FUN_0040ac43();
        FUN_0040ac38();
        local_8 = 1;
      }
      else {
        *(undefined4 *)(param_1 + 0x20) = *(undefined4 *)(*(int *)(param_1 + 0x20) + 4);
      }
    }
  }
  return local_8;
}
