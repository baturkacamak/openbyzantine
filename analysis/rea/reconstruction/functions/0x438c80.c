// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x438c80
// name: FUN_00438c80
// ranges: 0x438c80-0x438ccf
// calls: 0x438afa, 0x438c80

uint FUN_00438c80(void)

{
  uint uVar1;
  int local_8;
  
  if (DAT_004603d0 == 0) {
    local_8 = FUN_00438afa();
    if (local_8 == 5) {
      local_8 = 3;
    }
    uVar1 = (&DAT_00460670)[local_8];
  }
  else {
    uVar1 = (uint)DAT_00482428;
  }
  return uVar1;
}
