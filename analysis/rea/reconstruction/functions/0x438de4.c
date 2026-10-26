// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x438de4
// name: FUN_00438de4
// ranges: 0x438de4-0x439348
// calls: 0x417a17, 0x422927, 0x42295d, 0x438de4, 0x44ba00, 0x44c2f0, 0x44c580

void __cdecl FUN_00438de4(int param_1)

{
  int iVar1;
  uint uVar2;
  uint local_2c;
  uint local_28;
  uint local_24;
  uint local_20;
  uint local_18;
  char *local_14;
  uint local_10;
  uint local_c;
  int local_8;
  
  local_c = 0;
  local_24 = 0;
  local_10 = 0;
  local_28 = 0;
  local_2c = 0;
  local_18 = 0;
  DAT_0045d1d0 = 0;
  DAT_0045c060 = 0;
  for (local_20 = 0; (int)local_20 < 0x15e; local_20 = local_20 + 1) {
    if (*(int *)(&DAT_004649e8 + local_20 * 4) != 0) {
      local_c = local_c + 1;
    }
    if (*(int *)(&DAT_00464300 + local_20 * 4) != 0) {
      local_24 = local_24 + 1;
    }
    if (*(int *)(&DAT_00464f60 + local_20 * 4) != 0) {
      local_10 = local_10 + 1;
    }
    if (*(int *)(&DAT_00486a10 + local_20 * 4) != 0) {
      local_28 = local_28 + 1;
    }
    if (*(int *)(&DAT_00486220 + local_20 * 4) != 0) {
      local_18 = local_18 + 1;
    }
    if (*(int *)(&DAT_00485c90 + local_20 * 4) != 0) {
      local_2c = local_2c + 1;
    }
  }
  iVar1 = (local_2c + local_28 + local_10 + local_24 + local_c + local_18) * 2;
  local_14 = (char *)(iVar1 + 0xd);
  if ((DAT_00464878 == 0) && (DAT_0045f214 != (uint *)0x0)) {
    local_14 = FUN_0044c580(DAT_0045f214);
    local_14 = local_14 + iVar1 + 0xe;
  }
  DAT_004603d0 = (uint *)FUN_0044c2f0((int)local_14 << 2);
  *DAT_004603d0 = (uint)DAT_00482428;
  DAT_004603d0[1] = (uint)DAT_0048253e;
  DAT_004603d0[2] = DAT_00480330;
  DAT_004603d0[3] = local_c;
  local_8 = 4;
  for (local_20 = 0; (int)local_20 < 0x15e; local_20 = local_20 + 1) {
    if (*(int *)(&DAT_004649e8 + local_20 * 4) != 0) {
      DAT_004603d0[local_8] = local_20;
      DAT_004603d0[local_8 + 1] = *(uint *)(&DAT_004649e8 + local_20 * 4);
      local_8 = local_8 + 2;
      *(undefined4 *)(&DAT_004649e8 + local_20 * 4) = 0;
    }
  }
  DAT_004603d0[local_8] = local_24;
  local_8 = local_8 + 1;
  for (local_20 = 0; (int)local_20 < 0x15e; local_20 = local_20 + 1) {
    if (*(int *)(&DAT_00464300 + local_20 * 4) != 0) {
      DAT_004603d0[local_8] = local_20;
      DAT_004603d0[local_8 + 1] = *(uint *)(&DAT_00464300 + local_20 * 4);
      local_8 = local_8 + 2;
      *(undefined4 *)(&DAT_00464300 + local_20 * 4) = 0;
    }
  }
  DAT_004603d0[local_8] = local_10;
  local_8 = local_8 + 1;
  for (local_20 = 0; (int)local_20 < 0x15e; local_20 = local_20 + 1) {
    if (*(int *)(&DAT_00464f60 + local_20 * 4) != 0) {
      DAT_004603d0[local_8] = local_20;
      DAT_004603d0[local_8 + 1] = *(uint *)(&DAT_00464f60 + local_20 * 4);
      local_8 = local_8 + 2;
      *(undefined4 *)(&DAT_00464f60 + local_20 * 4) = 0;
    }
  }
  DAT_004603d0[local_8] = local_28;
  local_8 = local_8 + 1;
  for (local_20 = 0; (int)local_20 < 0x15e; local_20 = local_20 + 1) {
    if (*(int *)(&DAT_00486a10 + local_20 * 4) != 0) {
      DAT_004603d0[local_8] = local_20;
      DAT_004603d0[local_8 + 1] = *(uint *)(&DAT_00486a10 + local_20 * 4);
      local_8 = local_8 + 2;
      *(undefined4 *)(&DAT_00486a10 + local_20 * 4) = 0;
    }
  }
  DAT_004603d0[local_8] = DAT_0046066c;
  DAT_0046066c = 0;
  uVar2 = FUN_0042295d();
  DAT_004603d0[local_8 + 1] = uVar2;
  uVar2 = FUN_00422927();
  DAT_004603d0[local_8 + 2] = uVar2;
  if (-1 < (int)DAT_004603d0[local_8 + 2]) {
    local_2c = local_2c - 1;
  }
  if (param_1 != 0) {
    DAT_004603d0[local_8 + 2] = 0xffffffff;
  }
  DAT_004603d0[local_8 + 3] = local_18;
  local_8 = local_8 + 4;
  for (local_20 = 0; (int)local_20 < 0x15e; local_20 = local_20 + 1) {
    if (*(int *)(&DAT_00486220 + local_20 * 4) != 0) {
      DAT_004603d0[local_8] = local_20;
      DAT_004603d0[local_8 + 1] = *(uint *)(&DAT_00486220 + local_20 * 4);
      local_8 = local_8 + 2;
      *(undefined4 *)(&DAT_00486220 + local_20 * 4) = 0;
    }
  }
  DAT_004603d0[local_8] = local_2c;
  local_8 = local_8 + 1;
  for (local_20 = 0; (int)local_20 < 0x15e; local_20 = local_20 + 1) {
    if (*(int *)(&DAT_00485c90 + local_20 * 4) != 0) {
      DAT_004603d0[local_8] = local_20;
      DAT_004603d0[local_8 + 1] = *(uint *)(&DAT_00485c90 + local_20 * 4);
      local_8 = local_8 + 2;
      *(undefined4 *)(&DAT_00485c90 + local_20 * 4) = 0;
    }
  }
  if (DAT_00464878 == 0) {
    if (DAT_0045f214 == (uint *)0x0) {
      DAT_004603d0[local_8] = 0;
    }
    else {
      DAT_004603d0[local_8] = 1;
      FUN_0044ba00(DAT_004603d0 + local_8 + 1,DAT_0045f214);
    }
    FUN_00417a17((uint *)0x0,2);
  }
  return;
}
