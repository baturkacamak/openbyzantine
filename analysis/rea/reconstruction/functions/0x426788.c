// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x426788
// name: FUN_00426788
// ranges: 0x426788-0x426891
// calls: 0x40ac38, 0x40ac43, 0x426788

undefined4 __cdecl FUN_00426788(int param_1,int *param_2,FILE *param_3,char param_4)

{
  uint uVar1;
  uint uVar2;
  undefined1 local_14;
  undefined4 local_8;
  
  local_8 = 0;
  uVar1 = _fgetc(param_3);
  uVar2 = _fgetc(param_3);
  if ((char)uVar1 == -1) {
    *param_2 = 0;
  }
  else {
    if (param_4 == '\0') {
      *param_2 = *(int *)(param_1 + 0x4c4 + (uVar1 & 0xff) * 4);
    }
    else {
      *param_2 = *(int *)(param_1 + 0x58c + (uVar1 & 0xff) * 4);
    }
    for (local_14 = 0; (*param_2 != 0 && ((uint)local_14 < (uVar2 & 0xff))); local_14 = local_14 + 1
        ) {
      *param_2 = *(int *)(*param_2 + 8);
    }
    if (*param_2 == 0) {
      FUN_0040ac43();
      FUN_0040ac38();
      local_8 = 1;
    }
  }
  return local_8;
}
