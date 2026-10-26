// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x42d1e2
// name: FUN_0042d1e2
// ranges: 0x42d1e2-0x42d2d8; 0x42d2de-0x42d2fd; 0x42d347-0x42d382
// calls: 0x42d1e2, 0x4599c0

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

ushort __cdecl FUN_0042d1e2(int param_1)

{
  ushort uVar1;
  
  for (uVar1 = 0; *(char *)((uint)uVar1 + param_1) == '-'; uVar1 = uVar1 + 2) {
    switch(*(undefined1 *)((uint)(ushort)(uVar1 + 1) + param_1)) {
    case 0x62:
      _DAT_0046e270 = 1;
      break;
    default:
      FUN_004599c0((byte *)s__s_is_an_illegal_option_00460c50);
      break;
    case 100:
      _DAT_0046e828 = 1;
      break;
    case 0x65:
      _DAT_00484308 = 1;
      break;
    case 0x6d:
      DAT_0047a8a2 = 1;
      break;
    case 0x6e:
      DAT_0047734a = 1;
      break;
    case 0x6f:
      DAT_00483070 = 1;
      break;
    case 0x73:
      DAT_00485c74 = 1;
      break;
    case 0x74:
      DAT_00485c6c = '\x01';
      break;
    case 0x76:
      DAT_00485990 = 1;
      break;
    case 0x77:
      DAT_0047a280 = 1;
      break;
    case 0x7a:
      _DAT_0046edf0 = 1;
    }
  }
  if (DAT_00485c6c == '\0') {
    DAT_00485c74 = 1;
  }
  else {
    _DAT_0046e828 = 0;
  }
  return uVar1;
}
