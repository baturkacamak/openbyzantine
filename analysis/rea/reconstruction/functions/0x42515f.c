// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x42515f
// name: FUN_0042515f
// ranges: 0x42515f-0x425217
// calls: 0x41d45a, 0x424dc0, 0x42515f, 0x44ba08, 0x44bc20, 0x44d360, 0x44d3a0

void __cdecl FUN_0042515f(int param_1)

{
  uint local_58 [20];
  FILE *local_8;
  
  if (param_1 == 0xbe) {
    FUN_0041d45a(local_58,0x3d);
    DAT_00484e30 = 0x7f;
    DAT_00484e31 = 0x20;
    DAT_00484e32 = 0;
    FUN_0044ba08((uint *)&DAT_00484e30,local_58);
    FUN_00424dc0(s_savegame_tempsave_btb_0045fe40);
  }
  else {
    local_8 = (FILE *)FUN_0044bc20(PTR_s_savegame_tempsave_btb_0045fdd8,&DAT_0045fe58);
    if (local_8 != (FILE *)0x0) {
      _fclose(local_8);
      FUN_0044d360(PTR_s_savegame_autosave_btb_0045fddc);
      FUN_0044d3a0(PTR_s_savegame_tempsave_btb_0045fdd8,PTR_s_savegame_autosave_btb_0045fddc);
    }
  }
  return;
}
