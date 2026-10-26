// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x416b4b
// name: FUN_00416b4b
// ranges: 0x416b4b-0x416d04
// calls: 0x416ad8, 0x416b4b, 0x44a460, 0x44ac40, 0x44ba00, 0x44bc20, 0x44c2f0

undefined4 __cdecl FUN_00416b4b(uint *param_1,int param_2,int param_3,int *param_4)

{
  ushort uVar1;
  undefined4 uVar2;
  char local_7c [2];
  undefined1 local_7a;
  undefined4 local_70;
  undefined1 local_6c;
  undefined1 local_6b;
  uint local_68 [20];
  int local_18;
  FILE *local_14;
  char *local_10;
  LPVOID local_c;
  
  _memset(local_7c,0,0x12);
  local_70 = CONCAT22((short)param_3,0x280);
  local_6c = 0x10;
  local_6b = 1;
  local_7a = 2;
  if (param_4 != (int *)0x0) {
    *param_4 = -1;
  }
  if (param_1 == (uint *)0x0) {
    DAT_0045f1e4 = DAT_0045f1e4 + 1;
    _sprintf((char *)local_68,s_screen_02d_tga_0045f1f0,DAT_0045f1e4);
  }
  else {
    FUN_0044ba00(local_68,param_1);
  }
  local_14 = (FILE *)FUN_0044bc20((char *)local_68,&DAT_0045f200);
  if (local_14 == (FILE *)0x0) {
    uVar2 = 1;
  }
  else {
    FUN_0044a460(local_7c,1,0x12,local_14);
    local_c = (LPVOID)FUN_0044c2f0(param_3 * 0x500);
    for (local_18 = 0; local_18 < param_3 * 0x280; local_18 = local_18 + 1) {
      uVar1 = FUN_00416ad8(*(ushort *)(param_2 + local_18 * 2));
      *(ushort *)((int)local_c + local_18 * 2) = uVar1;
    }
    local_10 = (char *)(((local_70 >> 0x10) - 1) * (local_70 & 0xffff) * 2 + (int)local_c);
    for (local_18 = 0; local_18 < (int)(local_70 >> 0x10); local_18 = local_18 + 1) {
      FUN_0044a460(local_10,2,local_70 & 0xffff,local_14);
      local_10 = local_10 + (local_70 & 0xffff) * -2;
    }
    _fclose(local_14);
    FUN_0044ac40(local_c);
    if (param_4 != (int *)0x0) {
      *param_4 = DAT_0045f1e4;
    }
    uVar2 = 0;
  }
  return uVar2;
}
