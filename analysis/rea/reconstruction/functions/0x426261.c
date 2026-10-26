// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x426261
// name: FUN_00426261
// ranges: 0x426261-0x42658e
// calls: 0x410669, 0x426261, 0x42658f, 0x426662, 0x426788, 0x426892, 0x44ba00

int __cdecl FUN_00426261(uint param_1,FILE *param_2)

{
  int iVar1;
  uint local_28 [4];
  int local_18;
  uint local_14;
  int local_10;
  undefined1 local_c;
  int local_8;
  
  local_8 = 0;
  iVar1 = _fgetc(param_2);
  if ((char)iVar1 == '\0') {
    local_18 = 0;
    local_c = 0;
    do {
      iVar1 = _fgetc(param_2);
      *(char *)((int)local_28 + local_18) = (char)iVar1;
      iVar1 = local_18;
      local_18 = local_18 + 1;
    } while (*(char *)((int)local_28 + iVar1) != '\0');
    iVar1 = FUN_00410669(param_1,&local_14,local_28);
    local_8 = local_8 + iVar1;
    if (local_8 == 0) {
      local_10 = *(int *)(&DAT_00482ad0 + local_14 * 4);
      __fread_lk((char *)(local_10 + 8),2,1,param_2);
      iVar1 = _fgetc(param_2);
      *(char *)(local_10 + 10) = (char)iVar1;
      iVar1 = _fgetc(param_2);
      *(char *)(local_10 + 0xb) = (char)iVar1;
      iVar1 = _fgetc(param_2);
      *(char *)(local_10 + 0xc) = (char)iVar1;
      iVar1 = _fgetc(param_2);
      *(char *)(local_10 + 0xd) = (char)iVar1;
      iVar1 = _fgetc(param_2);
      *(char *)(local_10 + 0xe) = (char)iVar1;
      __fread_lk((char *)(local_10 + 0xf),1,0x32,param_2);
      __fread_lk((char *)(local_10 + 0x41),1,0x32,param_2);
      __fread_lk((char *)(local_10 + 0x73),1,0x32,param_2);
      for (local_18 = 0; local_18 < 0x32; local_18 = local_18 + 1) {
        iVar1 = FUN_0042658f(*(int *)(local_10 + 0x4c4 + local_18 * 4),param_2);
        local_8 = local_8 + iVar1;
      }
      for (local_18 = 0; local_18 < 0x32; local_18 = local_18 + 1) {
        iVar1 = FUN_0042658f(*(int *)(local_10 + 0x58c + local_18 * 4),param_2);
        local_8 = local_8 + iVar1;
      }
      for (local_18 = 0; local_18 < 0x32; local_18 = local_18 + 1) {
        iVar1 = FUN_00426662(local_10,(int *)(local_18 * 4 + local_10 + 0xa8),param_2);
        local_8 = local_8 + iVar1;
      }
      for (local_18 = 0; local_18 < 0x32; local_18 = local_18 + 1) {
        iVar1 = FUN_00426662(local_10,(int *)(local_18 * 4 + local_10 + 0x170),param_2);
        local_8 = local_8 + iVar1;
      }
      for (local_18 = 0; local_18 < 0x32; local_18 = local_18 + 1) {
        iVar1 = FUN_00426662(local_10,(int *)(local_18 * 4 + local_10 + 0x238),param_2);
        local_8 = local_8 + iVar1;
      }
      for (local_18 = 0; local_18 < 0x32; local_18 = local_18 + 1) {
        iVar1 = FUN_00426788(local_10,(int *)(local_18 * 4 + local_10 + 0x3c8),param_2,'\0');
        local_8 = local_8 + iVar1;
      }
      __fread_lk((char *)(local_10 + 0x490),1,0x32,param_2);
      for (local_18 = 0; local_18 < 0x32; local_18 = local_18 + 1) {
        FUN_00426892(local_10,(int *)(local_18 * 4 + local_10 + 0x654),param_2);
      }
      FUN_0044ba00((uint *)(local_10 + 0x1e94),local_28);
    }
  }
  return local_8;
}
