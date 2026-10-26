// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x424b15
// name: FUN_00424b15
// ranges: 0x424b15-0x424c4d
// calls: 0x424b15, 0x44ac40, 0x44ba00, 0x44ba08, 0x44c2f0, 0x44c520, 0x44c580

void __cdecl FUN_00424b15(uint *param_1)

{
  uint *puVar1;
  char *pcVar2;
  char *pcVar3;
  char *local_1c;
  int local_18;
  int local_8;
  
  pcVar2 = FUN_0044c580(param_1);
  puVar1 = DAT_0045fdb0;
  if (DAT_0045fdb0 == (uint *)0x0) {
    DAT_0045fdb0 = (uint *)FUN_0044c2f0((SIZE_T)(pcVar2 + 1));
    FUN_0044ba00(DAT_0045fdb0,param_1);
  }
  else {
    pcVar3 = FUN_0044c580(DAT_0045fdb0);
    DAT_0045fdb0 = (uint *)FUN_0044c2f0((SIZE_T)(pcVar2 + 1 + (int)pcVar3 + DAT_0045fdb8));
    FUN_0044ba00(DAT_0045fdb0,puVar1);
    local_8 = 0;
    for (local_1c = FUN_0044c520((char *)puVar1,'\n'); local_1c != (char *)0x0;
        local_1c = FUN_0044c520(local_1c + 1,'\n')) {
      local_8 = local_8 + 1;
    }
    for (local_18 = local_8 % DAT_0045fdb8; local_18 < DAT_0045fdb8; local_18 = local_18 + 1) {
      FUN_0044ba08(DAT_0045fdb0,(uint *)&DAT_0045fdd4);
    }
    FUN_0044ba08(DAT_0045fdb0,param_1);
    FUN_0044ac40(puVar1);
  }
  DAT_0045fdb4 = DAT_0045fdb0;
  return;
}
