// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x407bd7
// name: FUN_00407bd7
// ranges: 0x407bd7-0x407c1c
// calls: 0x407bd7

uint __cdecl FUN_00407bd7(int param_1)

{
  uint in_EAX;
  uint uVar1;
  
  if (DAT_0045d1c8 == 0) {
    uVar1 = in_EAX & 0xffff0000;
  }
  else if (param_1 == 1) {
    uVar1 = *(int *)(DAT_0045d1c8 + 4) + 1;
  }
  else {
    uVar1 = CONCAT22((short)((uint)DAT_0045d1c8 >> 0x10),*(undefined2 *)(DAT_0045d1c8 + 4));
  }
  return uVar1;
}
