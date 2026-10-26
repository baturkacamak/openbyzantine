// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x416375
// name: FUN_00416375
// ranges: 0x416375-0x4163aa
// calls: 0x416375

int __cdecl FUN_00416375(ushort param_1)

{
  undefined4 local_8;
  
  local_8 = 0;
  for (; (param_1 & 1) == 0; param_1 = (short)param_1 >> 1) {
    local_8 = local_8 + 1;
  }
  return local_8;
}
