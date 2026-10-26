// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x420888
// name: FUN_00420888
// ranges: 0x420888-0x420976
// calls: 0x418040, 0x4207a6, 0x4207ef, 0x420888, 0x44a460

void __cdecl FUN_00420888(int param_1,FILE *param_2)

{
  undefined4 uVar1;
  undefined4 *puVar2;
  undefined4 local_88 [9];
  char *local_64;
  int local_1c;
  char *local_18;
  undefined4 local_14;
  undefined4 local_10;
  undefined4 local_c;
  undefined4 local_8;
  
  local_14 = 0;
  local_10 = 0;
  local_c = 0xa8;
  local_8 = 0xa8;
  if (DAT_0045f7c8 == (int *)0x0) {
    FUN_004207ef(param_1,param_2);
  }
  else {
    local_88[0] = 0x6c;
    puVar2 = &local_14;
    uVar1 = FUN_00418040(param_1);
    FUN_004207a6(DAT_0045f7c8,uVar1,puVar2);
    (**(code **)(*DAT_0045f7c8 + 100))(DAT_0045f7c8,0,local_88,1,0);
    local_18 = local_64;
    for (local_1c = 0; local_1c < 0xa8; local_1c = local_1c + 1) {
      FUN_0044a460(local_18,2,0xa8,param_2);
      local_18 = local_18 + 0x150;
    }
    (**(code **)(*DAT_0045f7c8 + 0x80))(DAT_0045f7c8,0);
  }
  return;
}
