// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x43c3ac
// name: FUN_0043c3ac
// ranges: 0x43c3ac-0x43c465
// calls: 0x4181c5, 0x41842b, 0x43c1a8, 0x43c2ef, 0x43c3ac

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __cdecl FUN_0043c3ac(ushort *param_1)

{
  FUN_0043c2ef(*param_1);
  FUN_0043c1a8((short *)param_1);
  if ((DAT_0046edf4 == '\x01') && (DAT_0045dad8 = 1, DAT_0045dad4 != 0)) {
    DAT_0045dad4 = 0;
    FUN_0041842b(DAT_0046e82c,0x3e800);
  }
  else if (DAT_0046caf8 != 0xffff) {
    if (DAT_0046cb00 < 0x280) {
      DAT_0046cb00 = DAT_0046cb00 + 1;
    }
    _DAT_0046cb04 = _DAT_0046cb04 + 1;
    FUN_004181c5(DAT_00486888,&DAT_0046caf8,DAT_0046caf8,DAT_0046cafc);
  }
  return;
}
