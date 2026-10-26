// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4365b5
// name: FUN_004365b5
// ranges: 0x4365b5-0x4365f9
// calls: 0x4365b5

void __cdecl FUN_004365b5(int param_1,undefined1 param_2)

{
  int *local_8;
  
  for (local_8 = DAT_0047a89c; local_8 != (int *)0x0; local_8 = (int *)local_8[2]) {
    if (*local_8 == param_1) {
      *(undefined1 *)((int)local_8 + 7) = param_2;
    }
  }
  return;
}
