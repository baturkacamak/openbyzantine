// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x416fb8
// name: FUN_00416fb8
// ranges: 0x416fb8-0x417053
// calls: 0x416fb8, 0x417a17, 0x42024d

void FUN_00416fb8(void)

{
  FUN_00417a17((uint *)0x0,2);
  if (DAT_0045f218 != (int *)0x0) {
    (**(code **)(*DAT_0045f218 + 8))(DAT_0045f218);
    DAT_0045f218 = (int *)0x0;
  }
  if (DAT_0045f21c != (int *)0x0) {
    (**(code **)(*DAT_0045f21c + 8))(DAT_0045f21c);
    DAT_0045f21c = (int *)0x0;
  }
  FUN_0042024d(0x46c520,4);
  FUN_0042024d(0x46c538,0x20);
  FUN_0042024d(0x46c5b8,6);
  DAT_0045f220 = 0;
  return;
}
