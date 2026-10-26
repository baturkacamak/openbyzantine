// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4066c6
// name: FUN_004066c6
// ranges: 0x4066c6-0x4067e1
// calls: 0x4066c6

undefined4 __cdecl FUN_004066c6(uint param_1,uint param_2,int param_3)

{
  int iVar1;
  int iVar2;
  uint uVar3;
  
  iVar1 = param_3 + 1;
  iVar2 = 0x100 - iVar1;
  uVar3 = ((uint)(*(int *)(&DAT_00487710 +
                          iVar1 * 4 + ((param_1 & 0xffff) >> (DAT_00485c80 & 0x1f) & 0x1f) * 0x800)
                 + *(int *)(&DAT_00487710 +
                           iVar2 * 4 + ((param_2 & 0xffff) >> (DAT_00485c80 & 0x1f) & 0x1f) * 0x800)
                 ) >> 8) << (DAT_00485c80 & 0x1f) & (int)DAT_00485c86 |
          ((uint)(*(int *)(&DAT_00487710 +
                          iVar1 * 4 + ((param_1 & 0xffff) >> (DAT_00485c78 & 0x1f) & 0x1f) * 0x800)
                 + *(int *)(&DAT_00487710 +
                           iVar2 * 4 + ((param_2 & 0xffff) >> (DAT_00485c78 & 0x1f) & 0x1f) * 0x800)
                 ) >> 8) << (DAT_00485c78 & 0x1f);
  return CONCAT22((short)(uVar3 >> 0x10),
                  (ushort)uVar3 |
                  (ushort)((uint)(*(int *)(&DAT_00487710 + iVar1 * 4 + (param_1 & 0x1f) * 0x800) +
                                 *(int *)(&DAT_00487710 + iVar2 * 4 + (param_2 & 0x1f) * 0x800)) >>
                          8) & 0x1f);
}
