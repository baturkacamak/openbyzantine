// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x41d235
// name: FUN_0041d235
// ranges: 0x41d235-0x41d2c0
// calls: 0x41d235, 0x44ac40

void FUN_0041d235(void)

{
  if (DAT_00485c6c != '\x01') {
    if (DAT_0048242c != (FILE *)0x0) {
      _fclose(DAT_0048242c);
    }
    if (DAT_004859d0 != -1) {
      __close(DAT_004859d0);
    }
    if (DAT_00483604 != (LPVOID)0x0) {
      FUN_0044ac40(DAT_00483604);
    }
    if (DAT_00483624 != (LPVOID)0x0) {
      FUN_0044ac40(DAT_00483624);
    }
  }
  return;
}
