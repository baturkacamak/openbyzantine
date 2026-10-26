// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x42d8ba
// name: FUN_0042d8ba
// ranges: 0x42d8ba-0x42d914
// calls: 0x42d8ba
// port: Script return [partial]

void __cdecl FUN_0042d8ba(undefined4 *param_1)

{
  while (((*(char *)*param_1 == 'o' || (*(char *)*param_1 == -0x7d)) && (DAT_004848a8 != 0))) {
    DAT_004848a8 = DAT_004848a8 + -1;
    *param_1 = *(undefined4 *)(&DAT_0047a830 + DAT_004848a8 * 4);
  }
  return;
}
