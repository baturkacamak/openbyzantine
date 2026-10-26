// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x451cb0
// name: FUN_00451cb0
// ranges: 0x451cb0-0x451d84
// calls: 0x451cb0, 0x456340, 0x4565a0

int __cdecl FUN_00451cb0(int param_1,int param_2,int param_3,int param_4,int param_5,int param_6)

{
  int iVar1;
  uint uVar2;
  int iVar3;
  undefined1 local_24 [8];
  int local_1c;
  int local_14;
  uint local_10;
  int local_8;
  
  uVar2 = param_1 - 0x76c;
  if ((0x45 < (int)uVar2) && ((int)uVar2 < 0x8b)) {
    iVar1 = *(int *)(&DAT_00463f64 + param_2 * 4) + param_3;
    if (((uVar2 & 3) == 0) && (2 < param_2)) {
      iVar1 = iVar1 + 1;
    }
    FUN_00456340();
    local_14 = param_2 + -1;
    local_1c = param_4;
    iVar3 = ((param_4 + ((param_1 - 0x76dU & 0xfffffffc) + (uVar2 * 0x16d + iVar1) * 4) * 6) * 0x3c
            + param_5) * 0x3c + DAT_00463f00 + 0x7c558180 + param_6;
    if (DAT_00463f04 != 0) {
      local_10 = uVar2;
      local_8 = iVar1;
      iVar1 = FUN_004565a0((int)local_24);
      if (iVar1 != 0) {
        iVar3 = iVar3 + -0xe10;
      }
    }
    return iVar3;
  }
  return -1;
}
