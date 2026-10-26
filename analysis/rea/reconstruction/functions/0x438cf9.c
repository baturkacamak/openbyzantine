// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x438cf9
// name: FUN_00438cf9
// ranges: 0x438cf9-0x438d77
// calls: 0x438cf9
// port: Suspend hotspots [bounded]

void FUN_00438cf9(void)

{
  int local_8;
  
  for (local_8 = 0; local_8 < 0x15e; local_8 = local_8 + 1) {
    if ((*(int *)(&DAT_00464300 + local_8 * 4) == 0) ||
       (*(char *)(*(int *)(&DAT_00464300 + local_8 * 4) + 8) != '\x01')) {
      (&DAT_00464888)[local_8] = 0;
    }
    else {
      (&DAT_00464888)[local_8] = 1;
      *(undefined1 *)(*(int *)(&DAT_00464300 + local_8 * 4) + 8) = 0;
    }
  }
  return;
}
