// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x421949
// name: FUN_00421949
// ranges: 0x421949-0x421a5c
// calls: 0x40df05, 0x41842b, 0x4215d6, 0x4216fe, 0x421949, 0x42b2db

void __cdecl FUN_00421949(int param_1,int param_2)

{
  if (param_1 == 0) {
    DAT_0045f7e4 = 0;
    DAT_0045f7e8 = FUN_004215d6(DAT_0045f7d4);
  }
  else if (param_1 == 0x25) {
    DAT_0045f7e4 = DAT_0045f7e4 + 2;
  }
  else if (param_1 == 0x26) {
    DAT_0045f7e4 = DAT_0045f7e4 - 2;
  }
  DAT_0046c5f6 = 0 < (int)DAT_0045f7e4;
  DAT_0046c5f5 = (int)(DAT_0045f7e4 + 2) < DAT_0045f7e8;
  if (param_1 != -1) {
    if (param_2 != 0) {
      FUN_0042b2db(DAT_00477344,DAT_0047a8a4,0x4b000);
    }
    FUN_004216fe(DAT_0045f7e4);
    FUN_004216fe(DAT_0045f7e4 + 1);
    if (param_2 != 0) {
      FUN_0041842b((int)DAT_00477344,0x4b000);
      FUN_0040df05(0x1d);
    }
  }
  return;
}
