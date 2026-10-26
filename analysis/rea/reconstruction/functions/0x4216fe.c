// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4216fe
// name: FUN_004216fe
// ranges: 0x4216fe-0x421948
// calls: 0x4063e0, 0x40701f, 0x40711b, 0x418040, 0x41d45a, 0x421690, 0x4216fe

void __cdecl FUN_004216fe(uint param_1)

{
  int *piVar1;
  int iVar2;
  int local_80;
  int local_78;
  int local_70;
  char local_6c [8];
  uint local_64 [20];
  uint *local_14;
  int *local_10;
  int local_c;
  int local_8;
  
  local_10 = (int *)FUN_00418040(DAT_00477344);
  piVar1 = *(int **)(&DAT_0046c8d0 + param_1 * 4);
  if (piVar1 != (int *)0x0) {
    if ((param_1 & 1) == 0) {
      local_70 = 0x7f;
      local_78 = 0x61;
      local_c = 0x46;
    }
    else {
      local_70 = 0x16c;
      local_78 = 0x69;
      local_c = 0x4d;
    }
    _sprintf(local_6c,&DAT_0045fce4,piVar1[2] + 1);
    if (*piVar1 == -1) {
      FUN_0041d45a(local_64,0x3b);
      local_14 = local_64;
    }
    else {
      local_14 = (uint *)FUN_00421690(*piVar1);
    }
    FUN_004063e0(local_6c,local_70 + 0x14,local_c,2,local_10,0,2,1);
    FUN_004063e0((char *)local_14,local_70 + 0x14,local_c,2,local_10,0,0,1);
    for (local_80 = 0; local_80 < piVar1[1]; local_80 = local_80 + 1) {
      iVar2 = *(int *)(piVar1[3] + local_80 * 4);
      if (iVar2 == -1) {
        local_14 = DAT_0045f4e4;
        local_8 = 1;
      }
      else if (iVar2 == -3) {
        FUN_0041d45a(local_64,0x33);
        local_14 = local_64;
        local_8 = 0;
      }
      else {
        local_14 = *(uint **)(&DAT_0046c840 + iVar2 * 4);
        local_8 = 0;
      }
      FUN_004063e0((char *)local_14,local_70,local_78,2,local_10,local_8,0,99);
      local_78 = local_78 + 0x14;
      local_14 = *(uint **)(piVar1[4] + local_80 * 4);
      iVar2 = FUN_0040701f(local_14,0xb9,2);
      if (iVar2 * 0x14 + local_78 < 0x1e0) {
        FUN_004063e0((char *)local_14,local_70 + 10,local_78,2,local_10,local_8,3,99);
      }
      FUN_0040711b((char *)local_14);
      local_78 = local_78 + iVar2 * 0x14;
      if (0x1df < local_78) {
        return;
      }
    }
  }
  return;
}
