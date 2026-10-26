// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x405b90
// name: FUN_00405b90
// ranges: 0x405b90-0x405bf3
// calls: 0x405b90

void FUN_00405b90(void)

{
  int local_c;
  int local_8;
  
  for (local_8 = 0; local_8 < 0x21; local_8 = local_8 + 1) {
    for (local_c = 0; local_c < 0x101; local_c = local_c + 1) {
      *(int *)(&DAT_00487710 + local_c * 4 + local_8 * 0x800) = local_c * local_8;
    }
  }
  return;
}
