// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x42aebf
// name: FUN_0042aebf
// ranges: 0x42aebf-0x42b038
// calls: 0x42aa1a, 0x42aebf, 0x42b039, 0x44bc20

undefined4 __cdecl FUN_0042aebf(char *param_1,undefined4 *param_2,int *param_3,int *param_4)

{
  undefined4 uVar1;
  undefined4 local_a8;
  undefined4 local_a4;
  int local_a0;
  int local_9c;
  undefined4 local_40;
  int local_3c;
  int *local_38;
  FILE *local_34;
  char *local_30 [3];
  HGLOBAL local_24;
  int local_20;
  int local_1c;
  uint local_18;
  int local_8;
  
  local_34 = (FILE *)FUN_0044bc20(param_1,&DAT_004603cc);
  if (local_34 == (FILE *)0x0) {
    uVar1 = 1;
  }
  else {
    _fclose(local_34);
    _memset(local_30,0,0x28);
    local_30[0] = param_1;
    local_8 = FUN_0042b039(local_30);
    if (local_8 == 0) {
      uVar1 = 1;
    }
    else {
      _memset(&local_a8,0,0x6c);
      local_a8 = 0x6c;
      local_a4 = 7;
      local_40 = 0x840;
      local_9c = local_20;
      local_a0 = local_1c;
      local_3c = (**(code **)(*DAT_00486a04 + 0x18))(DAT_00486a04,&local_a8,param_2,0);
      if (local_3c == 0) {
        local_38 = (int *)*param_2;
        local_3c = (**(code **)(*local_38 + 0x58))(local_38,&local_a8);
        if ((local_9c == local_20) && (local_1c == local_a0)) {
          local_8 = FUN_0042aa1a(local_38,(int)local_24,local_20,local_1c,local_18,0);
          GlobalFree(local_24);
          *param_3 = local_20;
          *param_4 = local_1c;
          uVar1 = 0;
        }
        else {
          uVar1 = 1;
        }
      }
      else {
        uVar1 = 1;
      }
    }
  }
  return uVar1;
}
