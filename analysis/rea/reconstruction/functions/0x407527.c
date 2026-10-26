// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x407527
// name: FUN_00407527
// ranges: 0x407527-0x4075a0
// calls: 0x407420, 0x407527

void __cdecl FUN_00407527(int param_1)

{
  if (6 < *(int *)(&DAT_00468cc0 + DAT_0045d1c4 * 4)) {
    if (param_1 == 0) {
      if (0 < DAT_0045d1bc) {
        DAT_0045d1bc = DAT_0045d1bc + -1;
      }
    }
    else if ((param_1 == 1) && (DAT_0045d1bc < *(int *)(&DAT_00468cc0 + DAT_0045d1c4 * 4) + -6)) {
      DAT_0045d1bc = DAT_0045d1bc + 1;
    }
    FUN_00407420(1);
  }
  return;
}
