// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x40617c
// name: FUN_0040617c
// ranges: 0x40617c-0x4061e6
// calls: 0x40617c, 0x44ac40

void __cdecl FUN_0040617c(LPVOID param_1,int param_2)

{
  if (*(int *)(&DAT_00487538 + param_2 * 0x48) != 0) {
    FUN_0044ac40(*(LPVOID *)(&DAT_00487538 + param_2 * 0x48));
  }
  if (*(int *)(&DAT_00487550 + param_2 * 0x48) != 0) {
    FUN_0044ac40(*(LPVOID *)(&DAT_00487550 + param_2 * 0x48));
  }
  FUN_0044ac40(param_1);
  return;
}
