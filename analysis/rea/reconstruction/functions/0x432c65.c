// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x432c65
// name: FUN_00432c65
// ranges: 0x432c65-0x432c9b
// calls: 0x432c65, 0x436652

void __cdecl FUN_00432c65(uint param_1,int param_2)

{
  int iVar1;
  ushort uVar2;
  
  iVar1 = *(int *)(&DAT_004649e8 + (param_1 & 0xffff) * 4);
  uVar2 = FUN_00436652(param_2,1);
  *(uint *)(iVar1 + 0x2c) = (uint)uVar2;
  return;
}
