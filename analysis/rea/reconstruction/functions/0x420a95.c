// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x420a95
// name: FUN_00420a95
// ranges: 0x420a95-0x420b98
// calls: 0x4202c7, 0x420a63, 0x420a95, 0x44ac40

void FUN_00420a95(void)

{
  int local_8;
  
  FUN_00420a63();
  FUN_004202c7();
  if (DAT_0045f7c8 != (int *)0x0) {
    (**(code **)(*DAT_0045f7c8 + 8))(DAT_0045f7c8);
    DAT_0045f7c8 = (int *)0x0;
  }
  if (DAT_0045f4e4 != (LPVOID)0x0) {
    FUN_0044ac40(DAT_0045f4e4);
    DAT_0045f4e4 = (LPVOID)0x0;
  }
  for (local_8 = 0; local_8 < 0xf; local_8 = local_8 + 1) {
    if (*(int *)(&DAT_0046c888 + local_8 * 4) != 0) {
      FUN_0044ac40(*(LPVOID *)(&DAT_0046c888 + local_8 * 4));
      *(undefined4 *)(&DAT_0046c888 + local_8 * 4) = 0;
    }
  }
  for (local_8 = 0; local_8 < 0x11; local_8 = local_8 + 1) {
    if (*(int *)(&DAT_0046c840 + local_8 * 4) != 0) {
      FUN_0044ac40(*(LPVOID *)(&DAT_0046c840 + local_8 * 4));
      *(undefined4 *)(&DAT_0046c840 + local_8 * 4) = 0;
    }
  }
  return;
}
