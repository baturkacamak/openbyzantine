// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x41f753
// name: FUN_0041f753
// ranges: 0x41f753-0x41f7bd
// calls: 0x41f753, 0x44bc20

void __cdecl FUN_0041f753(undefined4 param_1)

{
  char local_10c [260];
  FILE *local_8;
  
  _sprintf(local_10c,s_savegame_gamepref__03d_0045faec,param_1);
  local_8 = (FILE *)FUN_0044bc20(local_10c,&DAT_0045fb04);
  if (local_8 != (FILE *)0x0) {
    __fread_lk(&DAT_0045f528,4,7,local_8);
    _fclose(local_8);
  }
  return;
}
