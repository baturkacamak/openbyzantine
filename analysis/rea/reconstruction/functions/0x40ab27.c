// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x40ab27
// name: FUN_0040ab27
// ranges: 0x40ab27-0x40abb3
// calls: 0x40ab27, 0x44c580

int __cdecl FUN_0040ab27(uint *param_1)

{
  char *pcVar1;
  char *local_10;
  int local_c;
  
  pcVar1 = FUN_0044c580(param_1);
  local_c = 0;
  for (local_10 = (char *)0x0; local_10 < pcVar1; local_10 = local_10 + 1) {
    if (('/' < local_10[(int)param_1]) && (local_10[(int)param_1] < ':')) {
      local_c = local_c * 10 + local_10[(int)param_1] + -0x30;
    }
  }
  return local_c;
}
