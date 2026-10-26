// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4102ae
// name: FUN_004102ae
// ranges: 0x4102ae-0x4102f6
// calls: 0x4102ae, 0x4102f7

void FUN_004102ae(void)

{
  int local_8;
  
  for (local_8 = 0; local_8 < 0x15e; local_8 = local_8 + 1) {
    FUN_004102f7(local_8);
    *(undefined4 *)(&DAT_004649e8 + local_8 * 4) = 0;
  }
  return;
}
