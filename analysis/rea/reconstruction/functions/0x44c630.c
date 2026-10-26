// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x44c630
// name: FUN_0044c630
// ranges: 0x44c630-0x44c653
// calls: 0x44c630

char * __cdecl FUN_0044c630(char *param_1,char *param_2,int param_3)

{
  char *pcVar1;
  char *pcVar2;
  
  pcVar1 = param_1;
  if (param_3 != 0) {
    do {
      pcVar2 = pcVar1;
      if (*param_2 == '\0') break;
      pcVar2 = pcVar1 + 1;
      *pcVar1 = *param_2;
      param_3 = param_3 + -1;
      param_2 = param_2 + 1;
      pcVar1 = pcVar2;
    } while (param_3 != 0);
    for (; param_3 != 0; param_3 = param_3 + -1) {
      *pcVar2 = '\0';
      pcVar2 = pcVar2 + 1;
    }
  }
  return param_1;
}
