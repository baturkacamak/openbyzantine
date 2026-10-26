// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x41f065
// name: FUN_0041f065
// ranges: 0x41f065-0x41f1ab
// calls: 0x41f065, 0x41f41c, 0x41fd8a, 0x44ba00, 0x44bc20, 0x44c2f0, 0x44d340

void __cdecl FUN_0041f065(int param_1)

{
  int iVar1;
  undefined4 uVar2;
  uint local_18 [2];
  FILE *local_10;
  int local_8;
  
  FUN_0041fd8a();
  DAT_0046c884 = 0;
  local_10 = (FILE *)FUN_0044bc20(s_savegame_autosave_btb_0045fa24,&DAT_0045fa20);
  if (local_10 != (FILE *)0x0) {
    _fclose(local_10);
    uVar2 = FUN_0044c2f0(0xd);
    (&DAT_0046c660)[DAT_0046c884] = uVar2;
    iVar1 = DAT_0046c884;
    DAT_0046c884 = DAT_0046c884 + 1;
    FUN_0044ba00((uint *)(&DAT_0046c660)[iVar1],(uint *)s_autosave_btb_0045fa3c);
  }
  local_10 = (FILE *)FUN_0044bc20(PTR_s_savegame_btb_sav_0045f480,&DAT_0045fa4c);
  if (local_10 != (FILE *)0x0) {
    do {
      if (((local_10->_flag & 0x10) != 0) ||
         (local_8 = FUN_0044d340(local_10,&DAT_0045fa50), local_8 != 1)) break;
      uVar2 = FUN_0044c2f0(8);
      (&DAT_0046c660)[DAT_0046c884] = uVar2;
      iVar1 = DAT_0046c884;
      DAT_0046c884 = DAT_0046c884 + 1;
      FUN_0044ba00((uint *)(&DAT_0046c660)[iVar1],local_18);
    } while (DAT_0046c884 != 100);
    _fclose(local_10);
  }
  if (param_1 != 0) {
    FUN_0041f41c();
  }
  return;
}
