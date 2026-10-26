// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x41f41c
// name: FUN_0041f41c
// ranges: 0x41f41c-0x41f594
// calls: 0x41f41c, 0x41fde8, 0x44ba00, 0x44bc20, 0x44c2f0, 0x44c580

void FUN_0041f41c(void)

{
  int iVar1;
  int iVar2;
  char *pcVar3;
  undefined4 uVar4;
  uint local_16c [18];
  int local_124;
  char local_120 [260];
  int local_1c;
  FILE *local_18;
  char local_14;
  char local_10 [4];
  int local_c;
  int local_8;
  
  local_c = DAT_0045f7cc * 10;
  iVar1 = local_c + 10;
  FUN_0041fde8();
  local_8 = 0;
  for (local_1c = local_c; (local_1c < iVar1 && ((&DAT_0046c660)[local_1c] != 0));
      local_1c = local_1c + 1) {
    _sprintf(local_120,s_savegame__s_0045fa94,(&DAT_0046c660)[local_1c]);
    local_18 = (FILE *)FUN_0044bc20(local_120,&DAT_0045faa0);
    if (local_18 == (FILE *)0x0) {
      return;
    }
    __fread_lk(local_10,4,1,local_18);
    do {
      iVar2 = _fgetc(local_18);
      local_14 = (char)iVar2;
    } while (local_14 != '\0');
    local_124 = 0;
    do {
      iVar2 = _fgetc(local_18);
      *(char *)((int)local_16c + local_124) = (char)iVar2;
      iVar2 = local_124;
      local_124 = local_124 + 1;
    } while (*(char *)((int)local_16c + iVar2) != '\0');
    _fclose(local_18);
    pcVar3 = FUN_0044c580(local_16c);
    uVar4 = FUN_0044c2f0((SIZE_T)(pcVar3 + 1));
    *(undefined4 *)(&DAT_0046c5f8 + local_8 * 4) = uVar4;
    FUN_0044ba00(*(uint **)(&DAT_0046c5f8 + local_8 * 4),local_16c);
    local_8 = local_8 + 1;
  }
  return;
}
