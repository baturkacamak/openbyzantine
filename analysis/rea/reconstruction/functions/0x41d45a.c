// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x41d45a
// name: FUN_0041d45a
// ranges: 0x41d45a-0x41d522
// calls: 0x41d45a, 0x44ba00, 0x44bc20, 0x44c580, 0x44ce20

void __cdecl FUN_0041d45a(uint *param_1,int param_2)

{
  char *pcVar1;
  uint local_114 [65];
  int local_10;
  FILE *local_8;
  
  *(undefined1 *)param_1 = 0;
  FUN_0044ba00(local_114,(uint *)s_setupart_messages_txt_0045f8a8);
  local_8 = (FILE *)FUN_0044bc20((char *)local_114,&DAT_0045f8c0);
  if (local_8 != (FILE *)0x0) {
    FUN_0044ce20((char *)param_1,0x50,local_8);
    for (local_10 = 0; local_10 < param_2; local_10 = local_10 + 1) {
      FUN_0044ce20((char *)param_1,0x50,local_8);
    }
    _fclose(local_8);
  }
  pcVar1 = FUN_0044c580(param_1);
  if ((pcVar1 + -1)[(int)param_1] == '\n') {
    (pcVar1 + -1)[(int)param_1] = '\0';
  }
  return;
}
