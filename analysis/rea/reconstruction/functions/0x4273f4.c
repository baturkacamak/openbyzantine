// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4273f4
// name: FUN_004273f4
// ranges: 0x4273f4-0x427452
// calls: 0x4273f4, 0x44bc20

undefined4 FUN_004273f4(void)

{
  undefined4 local_c;
  FILE *local_8;
  
  local_8 = (FILE *)FUN_0044bc20(s_savegame_status_000_00460178,&DAT_00460174);
  if (local_8 == (FILE *)0x0) {
    local_c = 0x34;
  }
  else {
    __fread_lk((char *)&local_c,4,1,local_8);
    _fclose(local_8);
  }
  return local_c;
}
