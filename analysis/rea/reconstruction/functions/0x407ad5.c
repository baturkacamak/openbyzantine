// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x407ad5
// name: FUN_00407ad5
// ranges: 0x407ad5-0x407b9b
// calls: 0x407420, 0x407ad5, 0x407b9c

void __cdecl FUN_00407ad5(undefined1 *param_1)

{
  int local_c;
  int local_8;
  
  FUN_00407b9c((int)param_1);
  for (local_8 = 0; local_8 < 10; local_8 = local_8 + 1) {
    for (local_c = 0; local_c < *(int *)(&DAT_00468cc0 + local_8 * 4); local_c = local_c + 1) {
      if (**(char **)(&DAT_00486f90 + *(int *)(&DAT_00468d78 + local_c * 4 + local_8 * 0x578) * 4)
          == '\x03') {
        **(undefined1 **)
          (&DAT_00486f90 + *(int *)(&DAT_00468d78 + local_c * 4 + local_8 * 0x578) * 4) = 1;
      }
    }
  }
  *param_1 = 3;
  FUN_00407420(1);
  return;
}
