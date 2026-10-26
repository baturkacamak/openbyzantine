// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x425218
// name: FUN_00425218
// ranges: 0x425218-0x425524
// calls: 0x425218, 0x425525, 0x4255d1, 0x42571c, 0x425952, 0x44a460

int __cdecl FUN_00425218(int param_1,FILE *param_2)

{
  int iVar1;
  bool bVar2;
  int iVar3;
  undefined3 extraout_var;
  int local_10;
  int local_8;
  
  local_8 = 0;
  iVar1 = *(int *)(&DAT_00482ad0 + param_1 * 4);
  if (iVar1 == 0) {
    _fputc(0xff,param_2);
  }
  else {
    _fputc(0,param_2);
    _fputs((char *)(iVar1 + 0x1e94),param_2);
    _fputc(0,param_2);
    FUN_0044a460((char *)(iVar1 + 8),2,1,param_2);
    _fputc((uint)*(byte *)(iVar1 + 10),param_2);
    _fputc((uint)*(byte *)(iVar1 + 0xb),param_2);
    _fputc((uint)*(byte *)(iVar1 + 0xc),param_2);
    _fputc((uint)*(byte *)(iVar1 + 0xd),param_2);
    _fputc((uint)*(byte *)(iVar1 + 0xe),param_2);
    FUN_0044a460((char *)(iVar1 + 0xf),1,0x32,param_2);
    FUN_0044a460((char *)(iVar1 + 0x41),1,0x32,param_2);
    FUN_0044a460((char *)(iVar1 + 0x73),1,0x32,param_2);
    for (local_10 = 0; local_10 < 0x32; local_10 = local_10 + 1) {
      FUN_00425525(*(int *)(iVar1 + 0x4c4 + local_10 * 4),param_2);
    }
    for (local_10 = 0; local_10 < 0x32; local_10 = local_10 + 1) {
      FUN_00425525(*(int *)(iVar1 + 0x58c + local_10 * 4),param_2);
    }
    for (local_10 = 0; local_10 < 0x32; local_10 = local_10 + 1) {
      iVar3 = FUN_004255d1(iVar1,*(byte **)(iVar1 + 0xa8 + local_10 * 4),param_2);
      local_8 = local_8 + iVar3;
    }
    for (local_10 = 0; local_10 < 0x32; local_10 = local_10 + 1) {
      iVar3 = FUN_004255d1(iVar1,*(byte **)(iVar1 + 0x170 + local_10 * 4),param_2);
      local_8 = local_8 + iVar3;
    }
    for (local_10 = 0; local_10 < 0x32; local_10 = local_10 + 1) {
      iVar3 = FUN_004255d1(iVar1,*(byte **)(iVar1 + 0x238 + local_10 * 4),param_2);
      local_8 = local_8 + iVar3;
    }
    for (local_10 = 0; local_10 < 0x32; local_10 = local_10 + 1) {
      bVar2 = FUN_0042571c(iVar1,*(int *)(iVar1 + 0x3c8 + local_10 * 4),param_2,'\0',
                           *(byte *)(local_10 + 0x490 + iVar1));
      local_8 = local_8 + CONCAT31(extraout_var,bVar2);
    }
    FUN_0044a460((char *)(iVar1 + 0x490),1,0x32,param_2);
    for (local_10 = 0; local_10 < 0x32; local_10 = local_10 + 1) {
      FUN_00425952(*(byte **)(iVar1 + 0x654 + local_10 * 4),param_2);
    }
  }
  return local_8;
}
