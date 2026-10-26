// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x432a63
// name: FUN_00432a63
// ranges: 0x432a63-0x432ad1
// calls: 0x432a63, 0x436652

void __cdecl FUN_00432a63(uint param_1,int param_2)

{
  int iVar1;
  short sVar2;
  
  iVar1 = *(int *)(&DAT_004649e8 + (param_1 & 0xffff) * 4);
  sVar2 = FUN_00436652(param_2,1);
  *(short *)(iVar1 + 0x10) = sVar2;
  sVar2 = FUN_00436652(param_2,2);
  *(short *)(iVar1 + 0x12) = sVar2;
  if (*(char *)(param_2 + 4) != '\0') {
    sVar2 = FUN_00436652(param_2,3);
    *(short *)(iVar1 + 0xe) = sVar2;
  }
  return;
}
