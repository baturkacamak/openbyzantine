// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x426662
// name: FUN_00426662
// ranges: 0x426662-0x426787
// calls: 0x40ac38, 0x40ac43, 0x426662, 0x44c2f0

undefined4 __cdecl FUN_00426662(int param_1,int *param_2,FILE *param_3)

{
  int iVar1;
  int iVar2;
  uint uVar3;
  uint uVar4;
  undefined1 local_14;
  undefined4 local_10;
  undefined4 local_8;
  
  local_8 = 0;
  iVar1 = _fgetc(param_3);
  if ((char)iVar1 == -1) {
    *param_2 = 0;
  }
  else {
    if (*param_2 == 0) {
      iVar2 = FUN_0044c2f0(8);
      *param_2 = iVar2;
    }
    *(char *)*param_2 = (char)iVar1;
    uVar3 = _fgetc(param_3);
    uVar4 = _fgetc(param_3);
    local_10 = *(int *)(param_1 + 0x58c + (uVar3 & 0xff) * 4);
    local_14 = 0;
    for (; ((uint)local_14 < (uVar4 & 0xff) && (local_10 != 0)); local_10 = *(int *)(local_10 + 8))
    {
      local_14 = local_14 + 1;
    }
    if (local_10 == 0) {
      FUN_0040ac43();
      FUN_0040ac38();
      local_8 = 1;
    }
    else {
      *(int *)(*param_2 + 4) = local_10;
    }
  }
  return local_8;
}
