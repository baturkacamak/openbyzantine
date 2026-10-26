// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x432464
// name: FUN_00432464
// ranges: 0x432464-0x4324af
// calls: 0x432464

char * __cdecl FUN_00432464(char *param_1)

{
  char *pcVar1;
  char *pcVar2;
  
  pcVar1 = param_1;
  for (; (pcVar2 = pcVar1, param_1 != (char *)0x0 && (pcVar2 = param_1, *param_1 != 'o'));
      param_1 = *(char **)(param_1 + 0x2c)) {
  }
  return pcVar2;
}
