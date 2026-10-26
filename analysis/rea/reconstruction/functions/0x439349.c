// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x439349
// name: FUN_00439349
// ranges: 0x439349-0x43972f
// calls: 0x4085e7, 0x417a17, 0x426935, 0x427de1, 0x439349, 0x44ac40

void FUN_00439349(void)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int local_24;
  int local_8;
  
  DAT_0048253e = (undefined2)*(undefined4 *)((int)DAT_004603d0 + 4);
  DAT_00480330 = *(int *)((int)DAT_004603d0 + 8);
  iVar1 = *(int *)((int)DAT_004603d0 + 0xc);
  local_8 = 4;
  for (local_24 = 0; local_24 < iVar1; local_24 = local_24 + 1) {
    iVar2 = *(int *)((int)DAT_004603d0 + local_8 * 4);
    *(undefined4 *)(&DAT_004649e8 + iVar2 * 4) =
         *(undefined4 *)((int)DAT_004603d0 + (local_8 + 1) * 4);
    local_8 = local_8 + 2;
    if (*(int *)(&DAT_004649e8 + iVar2 * 4) == 0) {
      *(undefined1 *)(*(int *)(&DAT_0047dd70 + iVar2 * 4) + 9) = 0;
    }
    else {
      *(undefined1 *)(*(int *)(&DAT_0047dd70 + iVar2 * 4) + 9) = 1;
    }
  }
  iVar1 = *(int *)((int)DAT_004603d0 + local_8 * 4);
  local_8 = local_8 + 1;
  for (local_24 = 0; local_24 < iVar1; local_24 = local_24 + 1) {
    *(undefined4 *)(&DAT_00464300 + *(int *)((int)DAT_004603d0 + local_8 * 4) * 4) =
         *(undefined4 *)((int)DAT_004603d0 + (local_8 + 1) * 4);
    local_8 = local_8 + 2;
  }
  iVar1 = *(int *)((int)DAT_004603d0 + local_8 * 4);
  local_8 = local_8 + 1;
  for (local_24 = 0; local_24 < iVar1; local_24 = local_24 + 1) {
    *(undefined4 *)(&DAT_00464f60 + *(int *)((int)DAT_004603d0 + local_8 * 4) * 4) =
         *(undefined4 *)((int)DAT_004603d0 + (local_8 + 1) * 4);
    local_8 = local_8 + 2;
  }
  iVar1 = *(int *)((int)DAT_004603d0 + local_8 * 4);
  local_8 = local_8 + 1;
  for (local_24 = 0; local_24 < iVar1; local_24 = local_24 + 1) {
    *(undefined4 *)(&DAT_00486a10 + *(int *)((int)DAT_004603d0 + local_8 * 4) * 4) =
         *(undefined4 *)((int)DAT_004603d0 + (local_8 + 1) * 4);
    local_8 = local_8 + 2;
  }
  DAT_0046066c = *(undefined4 *)((int)DAT_004603d0 + local_8 * 4);
  DAT_0045fd2c = *(undefined4 *)((int)DAT_004603d0 + (local_8 + 1) * 4);
  iVar1 = *(int *)((int)DAT_004603d0 + (local_8 + 2) * 4);
  iVar2 = *(int *)((int)DAT_004603d0 + (local_8 + 3) * 4);
  local_8 = local_8 + 4;
  for (local_24 = 0; local_24 < iVar2; local_24 = local_24 + 1) {
    *(undefined4 *)(&DAT_00486220 + *(int *)((int)DAT_004603d0 + local_8 * 4) * 4) =
         *(undefined4 *)((int)DAT_004603d0 + (local_8 + 1) * 4);
    local_8 = local_8 + 2;
  }
  iVar2 = *(int *)((int)DAT_004603d0 + local_8 * 4);
  local_8 = local_8 + 1;
  for (local_24 = 0; local_24 < iVar2; local_24 = local_24 + 1) {
    iVar3 = *(int *)((int)DAT_004603d0 + local_8 * 4);
    *(undefined4 *)(&DAT_00485c90 + iVar3 * 4) =
         *(undefined4 *)((int)DAT_004603d0 + (local_8 + 1) * 4);
    local_8 = local_8 + 2;
    if (*(int *)(&DAT_00485c90 + iVar3 * 4) == 0) {
      *(undefined1 *)(*(int *)(&DAT_0047a8e0 + iVar3 * 4) + 9) = 0;
    }
    else {
      *(undefined1 *)(*(int *)(&DAT_0047a8e0 + iVar3 * 4) + 9) = 1;
    }
  }
  if (DAT_00464878 == 0) {
    if (*(int *)((int)DAT_004603d0 + local_8 * 4) == 0) {
      FUN_00417a17((uint *)0x0,2);
    }
    else {
      FUN_00417a17((uint *)((local_8 + 1) * 4 + (int)DAT_004603d0),0);
    }
  }
  FUN_0044ac40(DAT_004603d0);
  DAT_004603d0 = (LPVOID)0x0;
  DAT_0045c060 = (uint)(DAT_00480330 != -1);
  DAT_0045d1d0 = 1;
  FUN_004085e7();
  FUN_00426935();
  if ((iVar1 == -1) || (iVar1 == -2)) {
    DAT_0045fd2c = 0;
  }
  else {
    FUN_00427de1(iVar1,0);
  }
  return;
}
