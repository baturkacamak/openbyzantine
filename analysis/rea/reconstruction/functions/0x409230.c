// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x409230
// name: FUN_00409230
// ranges: 0x409230-0x4092af; 0x4092b5-0x4092cc; 0x4092dd-0x409340
// calls: 0x409230, 0x40a470, 0x4599c0

void FUN_00409230(void)

{
  int local_8;
  
  for (local_8 = DAT_0047a89c; local_8 != 0; local_8 = *(int *)(local_8 + 8)) {
    FUN_004599c0((byte *)s_____>_UNDOING_0045d328);
    switch(*(undefined1 *)(local_8 + 4)) {
    case 0x4a:
      FUN_004599c0(&DAT_0045d338);
      break;
    case 0x4b:
      FUN_004599c0(&DAT_0045d340);
      break;
    case 0x4c:
      FUN_004599c0((byte *)s_IFTOFF_0045d344);
      break;
    case 0x4d:
      FUN_004599c0((byte *)s_IFTON_0045d34c);
    }
    FUN_004599c0(&DAT_0045d354);
    FUN_0040a470(*(byte *)(local_8 + 5),'\0');
    FUN_004599c0(&DAT_0045d358);
    FUN_004599c0(&DAT_0045d35c);
    FUN_004599c0(&DAT_0045d360);
  }
  return;
}
