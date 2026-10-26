// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x420dc9
// name: FUN_00420dc9
// ranges: 0x420dc9-0x420f09
// calls: 0x4063e0, 0x418040, 0x4181c5, 0x420683, 0x420dc9

void __cdecl FUN_00420dc9(int param_1,int param_2)

{
  int iVar1;
  int *piVar2;
  char *pcVar3;
  int local_30;
  int local_2c;
  int local_18;
  int local_14;
  undefined4 local_10;
  undefined4 local_c;
  uint local_8;
  
  local_2c = 0x6c;
  local_18 = 0x172;
  local_14 = 0x6c;
  local_10 = 0x230;
  local_c = 0x1ac;
  piVar2 = (int *)FUN_00418040(DAT_00477344);
  if (param_2 != 0) {
    FUN_00420683(&local_18);
  }
  for (local_30 = 0; local_30 < 0x10; local_30 = local_30 + 1) {
    iVar1 = *(int *)(&DAT_0046c800 + local_30 * 4);
    if (iVar1 != -1) {
      local_8 = (uint)(iVar1 == param_1);
      pcVar3 = _strrchr(*(char **)(&DAT_0046c840 + iVar1 * 4),0x3a);
      if (pcVar3 != (char *)0x0) {
        *pcVar3 = '\0';
      }
      FUN_004063e0(*(char **)(&DAT_0046c840 + iVar1 * 4),0x172,local_2c,2,piVar2,local_8,0,1);
      if (pcVar3 != (char *)0x0) {
        *pcVar3 = ':';
      }
      local_2c = local_2c + 0x14;
    }
  }
  if (param_2 != 0) {
    FUN_004181c5(piVar2,&local_18,local_18,local_14);
  }
  return;
}
