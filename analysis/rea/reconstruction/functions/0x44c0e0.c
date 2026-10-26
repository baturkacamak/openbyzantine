// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x44c0e0
// name: FUN_0044c0e0
// ranges: 0x44c0e0-0x44c0ff
// calls: 0x44c0e0

void __cdecl FUN_0044c0e0(undefined4 *param_1,undefined4 *param_2)

{
  for (; param_1 < param_2; param_1 = param_1 + 1) {
    if ((code *)*param_1 != (code *)0x0) {
      (*(code *)*param_1)();
    }
  }
  return;
}
