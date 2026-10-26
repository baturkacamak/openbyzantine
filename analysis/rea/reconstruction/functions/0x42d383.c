// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x42d383
// name: FUN_0042d383
// ranges: 0x42d383-0x42d42a
// calls: 0x40abb4, 0x42d383, 0x436652
// port: Label table [bounded]

undefined2 FUN_0042d383(void)

{
  ushort uVar1;
  char *local_c;
  undefined2 local_8;
  
  local_8 = 0;
  for (local_c = DAT_0047a238; local_c != (char *)0x0; local_c = *(char **)(local_c + 0x2c)) {
    if (*local_c == -0x1f) {
      uVar1 = FUN_00436652((int)local_c,0);
      if (uVar1 < 0xdac) {
        *(undefined4 *)(&DAT_004654e0 + (uint)uVar1 * 4) = *(undefined4 *)(local_c + 0x2c);
      }
      else {
        FUN_0040abb4();
        local_8 = 1;
      }
    }
  }
  return local_8;
}
