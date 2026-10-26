// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x43299d
// name: FUN_0043299d
// ranges: 0x43299d-0x4329e3
// calls: 0x432904, 0x43299d, 0x436652

void __cdecl FUN_0043299d(uint param_1,int param_2)

{
  int iVar1;
  short sVar2;
  
  iVar1 = *(int *)(&DAT_004649e8 + (param_1 & 0xffff) * 4);
  sVar2 = FUN_00436652(param_2,1);
  *(short *)(iVar1 + 4) = sVar2;
  FUN_00432904(param_1,param_2,'\x02');
  return;
}
