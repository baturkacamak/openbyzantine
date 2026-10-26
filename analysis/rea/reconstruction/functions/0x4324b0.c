// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4324b0
// name: FUN_004324b0
// ranges: 0x4324b0-0x4325d2
// calls: 0x4324b0, 0x436652

void __cdecl FUN_004324b0(uint param_1,int param_2)

{
  short *psVar1;
  short sVar2;
  
  psVar1 = *(short **)(&DAT_004649e8 + (param_1 & 0xffff) * 4);
  if (((&DAT_00460428)[*(ushort *)(param_2 + 0xe)] != ' ') ||
     ((*(byte *)((int)psVar1 + 9) & 0x20) == 0)) {
    if (*(char *)(param_2 + 2) != '\0') {
      *(undefined *)((int)psVar1 + 9) = (&DAT_00460428)[*(ushort *)(param_2 + 0xe)];
    }
    if (*(char *)(param_2 + 3) != '\0') {
      sVar2 = FUN_00436652(param_2,2);
      psVar1[2] = sVar2;
    }
    if (*(char *)(param_2 + 4) != '\0') {
      sVar2 = FUN_00436652(param_2,3);
      *psVar1 = sVar2;
    }
    if (*(char *)(param_2 + 5) != '\0') {
      sVar2 = FUN_00436652(param_2,4);
      psVar1[1] = sVar2 + 0x14;
    }
    *(undefined4 *)(psVar1 + 0xe) = *(undefined4 *)(psVar1 + 0xc);
    psVar1[0x10] = 0;
    psVar1[0x11] = 0;
    psVar1[0x12] = *psVar1;
    psVar1[0x13] = psVar1[1];
    psVar1[3] = 0;
  }
  return;
}
