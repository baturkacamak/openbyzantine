// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x41f279
// name: FUN_0041f279
// ranges: 0x41f279-0x41f41b
// calls: 0x416974, 0x418040, 0x4181c5, 0x41f279, 0x420683, 0x420977, 0x44ba00, 0x44ba08, 0x44bc20

void __cdecl FUN_0041f279(int param_1)

{
  char *pcVar1;
  int iVar2;
  uint local_130 [65];
  FILE *local_2c;
  int local_28;
  int *local_24;
  char local_20;
  char local_1c [4];
  int local_18;
  int local_14;
  int local_10;
  undefined4 local_c;
  undefined4 local_8;
  
  local_24 = (int *)FUN_00418040(DAT_00477344);
  local_14 = 0x45;
  local_10 = 0x2c;
  local_c = 0xed;
  local_8 = 0xd4;
  local_18 = DAT_0045f7cc * 10;
  if (DAT_0045f7d0 == -1) {
    if (param_1 == -1) {
      FUN_00420683(&local_14);
    }
    else {
      iVar2 = param_1 + local_18;
      FUN_0044ba00(local_130,(uint *)s_savegame__0045fa6c);
      FUN_0044ba08(local_130,(uint *)(&DAT_0046c660)[iVar2]);
      local_2c = (FILE *)FUN_0044bc20((char *)local_130,&DAT_0045fa78);
      if (local_2c == (FILE *)0x0) {
        return;
      }
      __fread_lk(local_1c,4,1,local_2c);
      do {
        iVar2 = _fgetc(local_2c);
        local_20 = (char)iVar2;
      } while (local_20 != '\0');
      local_28 = 0;
      local_20 = '\0';
      do {
        iVar2 = _fgetc(local_2c);
        (&DAT_00484e30)[local_28] = (char)iVar2;
        iVar2 = local_28 + 1;
        pcVar1 = &DAT_00484e30 + local_28;
        local_28 = iVar2;
      } while (*pcVar1 != '\0');
      FUN_00420977(local_2c);
      _fclose(local_2c);
      (**(code **)(*local_24 + 0x1c))(local_24,0x45,0x2c,DAT_0045f7c8,0,0x10);
      FUN_00416974(s_setupart_pprclip_tga_0045fa7c,local_24,0xe2,0x27,1);
    }
    FUN_004181c5(local_24,&local_14,local_14,local_10);
  }
  return;
}
