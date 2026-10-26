// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x438d78
// name: FUN_00438d78
// ranges: 0x438d78-0x438de3
// calls: 0x438d78
// port: Restore hotspots [bounded]

void FUN_00438d78(void)

{
  int local_8;
  
  for (local_8 = 0; local_8 < 0x15e; local_8 = local_8 + 1) {
    if (((&DAT_00464888)[local_8] == '\x01') &&
       ((&DAT_00464888)[local_8] = 0, *(int *)(&DAT_00464300 + local_8 * 4) != 0)) {
      *(undefined1 *)(*(int *)(&DAT_00464300 + local_8 * 4) + 8) = 1;
    }
  }
  return;
}
