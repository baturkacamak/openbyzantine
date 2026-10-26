// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x42044d
// name: FUN_0042044d
// ranges: 0x42044d-0x420586
// calls: 0x40df05, 0x41f92e, 0x41f98e, 0x42044d

void __cdecl FUN_0042044d(int param_1)

{
  int local_c;
  int local_8;
  
  local_8 = DAT_0045f7cc * 10;
  for (local_c = 0x12; local_c < 0x1c; local_c = local_c + 1) {
    if ((&DAT_0046c660)[local_8] == 0) {
      (&DAT_0046c5d0)[local_c] = 0;
    }
    else {
      (&DAT_0046c5d0)[local_c] = 1;
    }
    local_8 = local_8 + 1;
  }
  DAT_0046c5ed = 0 < DAT_0045f7cc;
  DAT_0046c5ec = local_8 < DAT_0046c884;
  if (DAT_0045f7d0 == -1) {
    DAT_0046c5d1 = 0;
    DAT_0046c5d2 = 0;
    if (param_1 != 0) {
      FUN_0041f92e(1);
      FUN_0041f92e(2);
    }
    if ((DAT_0045f490 == 0) || (DAT_0045f490 == 3)) {
      DAT_0046c5d5 = 1;
    }
  }
  else {
    DAT_0046c5d1 = 1;
    DAT_0046c5d2 = 1;
    DAT_0046c5d5 = 0;
  }
  FUN_0040df05(0x1d);
  if (param_1 != 0) {
    FUN_0041f98e();
  }
  return;
}
