// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x454380
// name: FUN_00454380
// ranges: 0x454380-0x4543a9
// calls: 0x454380

byte __cdecl FUN_00454380(uint param_1)

{
  if (DAT_004a7710 <= param_1) {
    return 0;
  }
  return *(byte *)(*(int *)((int)&DAT_004a7720 + ((int)(param_1 & 0xffffffe7) >> 3)) + 4 +
                  (param_1 & 0x1f) * 8) & 0x40;
}
