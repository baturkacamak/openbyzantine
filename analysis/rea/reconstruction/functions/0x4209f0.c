// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4209f0
// name: FUN_004209f0
// ranges: 0x4209f0-0x420a62
// calls: 0x416710, 0x4209f0

undefined4 __cdecl FUN_004209f0(undefined4 *param_1,undefined4 *param_2)

{
  undefined4 local_8;
  
  local_8 = 0;
  if (DAT_0045f488 == 0) {
    local_8 = FUN_00416710(s_setupart_yelstky_tga_0045fc1c,&DAT_0045f488,1,&DAT_0046c8c4,
                           &DAT_0046c8c8);
  }
  if (param_1 != (undefined4 *)0x0) {
    *param_1 = DAT_0046c8c4;
  }
  if (param_2 != (undefined4 *)0x0) {
    *param_2 = DAT_0046c8c8;
  }
  return local_8;
}
