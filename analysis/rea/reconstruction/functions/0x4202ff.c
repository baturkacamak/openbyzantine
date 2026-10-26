// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4202ff
// name: FUN_004202ff
// ranges: 0x4202ff-0x42044c
// calls: 0x4063e0, 0x418040, 0x4181c5, 0x4202ff, 0x420683

void __cdecl FUN_004202ff(int param_1,int param_2)

{
  int iVar1;
  int *piVar2;
  int local_28;
  int local_24;
  int local_14;
  int local_10;
  undefined4 local_c;
  undefined4 local_8;
  
  local_24 = 0x42;
  local_14 = 0x17a;
  local_10 = 0x42;
  local_c = 0x242;
  local_8 = 0x16e;
  piVar2 = (int *)FUN_00418040(DAT_00477344);
  iVar1 = DAT_0045f7cc * 10;
  if (param_2 != 0) {
    FUN_00420683(&local_14);
  }
  for (local_28 = 0; (local_28 < 10 && (*(int *)(&DAT_0046c5f8 + local_28 * 4) != 0));
      local_28 = local_28 + 1) {
    if ((local_28 + iVar1 == DAT_0045f7d0) || ((local_28 == param_1 && (DAT_0045f7d0 == -1)))) {
      FUN_004063e0(*(char **)(&DAT_0046c5f8 + local_28 * 4),0x1de,local_24,2,piVar2,1,1,1);
    }
    else {
      FUN_004063e0(*(char **)(&DAT_0046c5f8 + local_28 * 4),0x1de,local_24,2,piVar2,0,1,1);
    }
    local_24 = local_24 + 0x1e;
  }
  if (param_2 != 0) {
    FUN_004181c5(piVar2,&local_14,local_14,local_10);
  }
  return;
}
