// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4565a0
// name: FUN_004565a0
// ranges: 0x4565a0-0x456662
// calls: 0x4565a0

undefined4 __cdecl FUN_004565a0(int param_1)

{
  uint uVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  
  uVar1 = *(uint *)(param_1 + 0x14);
  if ((((int)uVar1 < 0x43) || (iVar2 = *(int *)(param_1 + 0x10), iVar2 < 3)) || (9 < iVar2)) {
    return 0;
  }
  if ((3 < iVar2) && (iVar2 < 9)) {
    return 1;
  }
  if (((int)uVar1 < 0x57) || (iVar2 != 3)) {
    iVar4 = *(int *)(&DAT_00463f6c + iVar2 * 4);
  }
  else {
    iVar4 = iRam00463f74 + 7;
  }
  if ((uVar1 & 3) == 0) {
    iVar4 = iVar4 + 1;
  }
  iVar4 = iVar4 - (int)(((int)(uVar1 - 1) >> 2) + uVar1 * 0x16d + -0x63db + iVar4) % 7;
  iVar3 = *(int *)(param_1 + 0x1c);
  if (iVar2 == 3) {
    if ((iVar3 <= iVar4) && ((iVar4 != iVar3 || (*(int *)(param_1 + 8) < 2)))) {
      return 0;
    }
    return 1;
  }
  if ((iVar4 <= iVar3) && ((iVar4 != iVar3 || (0 < *(int *)(param_1 + 8))))) {
    return 0;
  }
  return 1;
}
