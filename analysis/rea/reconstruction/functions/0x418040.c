// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x418040
// name: FUN_00418040
// ranges: 0x418040-0x418099
// calls: 0x418040

undefined4 __cdecl FUN_00418040(int param_1)

{
  int local_8;
  
  if (DAT_0046e82c == param_1) {
    local_8 = 0;
  }
  else if (DAT_00477344 == param_1) {
    local_8 = 1;
  }
  else {
    local_8 = 2;
  }
  return (&DAT_004869e0)[local_8];
}
