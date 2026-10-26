// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x40f080
// name: FUN_0040f080
// ranges: 0x40f080-0x40f134
// calls: 0x40f080

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __cdecl FUN_0040f080(int *param_1,int param_2,ushort *param_3)

{
  int iVar1;
  
  if (_DAT_00485c80 == 6) {
    do {
      iVar1 = *param_1;
      param_1 = (int *)((int)param_1 + 3);
      *param_3 = (ushort)(iVar1 << 8) & 0xf800 | (ushort)iVar1 >> 5 & 0xffe0 |
                 (ushort)((uint)iVar1 >> 0x13) & 0x1f;
      param_2 = param_2 + -1;
      param_3 = param_3 + 1;
    } while (param_2 != 0);
  }
  else if (_DAT_00485c80 == 5) {
    do {
      iVar1 = *param_1;
      param_1 = (int *)((int)param_1 + 3);
      *param_3 = (ushort)(iVar1 << 7) & 0xf800 | (ushort)iVar1 >> 6 & 0xffe0 |
                 (ushort)((uint)iVar1 >> 0x13) & 0x1f;
      param_2 = param_2 + -1;
      param_3 = param_3 + 1;
    } while (param_2 != 0);
  }
  return;
}
