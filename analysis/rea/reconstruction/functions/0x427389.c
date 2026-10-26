// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x427389
// name: FUN_00427389
// ranges: 0x427389-0x4273f3
// calls: 0x427389, 0x4273f4, 0x44a460, 0x44bc20

void __cdecl FUN_00427389(int param_1)

{
  int iVar1;
  FILE *_File;
  
  iVar1 = FUN_004273f4();
  if ((iVar1 < param_1) &&
     (_File = (FILE *)FUN_0044bc20(s_savegame_status_000_00460160,&DAT_0046015c),
     _File != (FILE *)0x0)) {
    FUN_0044a460((char *)&param_1,4,1,_File);
    _fclose(_File);
  }
  return;
}
