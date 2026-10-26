// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x45a3d0
// name: FUN_0045a3d0
// ranges: 0x45a3d0-0x45a3fe
// calls: 0x45a3d0

void __cdecl FUN_0045a3d0(byte *param_1,int param_2,int *param_3)

{
  byte bVar1;
  
  do {
    if (param_2 < 1) {
      return;
    }
    bVar1 = *param_1;
    param_1 = param_1 + 1;
    write_char(bVar1,param_3);
    param_2 = param_2 + -1;
  } while (*param_3 != -1);
  return;
}
