// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4287ff
// name: FUN_004287ff
// ranges: 0x4287ff-0x4288e4
// calls: 0x41624a, 0x422a22, 0x4281b1, 0x4287ff

void FUN_004287ff(void)

{
  int *piVar1;
  int iVar2;
  int local_10;
  int local_8;
  
  while (DAT_00460198 != 0) {
    FUN_0041624a();
  }
  iVar2 = FUN_00422a22();
  if ((iVar2 != -1) && (-1 < iVar2)) {
    *(undefined4 *)(*(int *)(&DAT_00486220 + iVar2 * 4) + 0x34) = 1;
  }
  for (local_10 = 0; local_10 < 0x15e; local_10 = local_10 + 1) {
    if ((*(int *)(&DAT_00486220 + local_10 * 4) != 0) &&
       (iVar2 = FUN_004281b1(local_10), iVar2 != 0)) {
      *(undefined4 *)(*(int *)(&DAT_00486220 + local_10 * 4) + 0x34) = 1;
    }
  }
  for (local_8 = DAT_0046019c; local_8 != 0; local_8 = *(int *)(local_8 + 0x58)) {
    piVar1 = *(int **)(*(int *)(local_8 + 0x54) + 8);
    (**(code **)(*piVar1 + 0x48))(piVar1);
  }
  return;
}
