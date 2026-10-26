// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4553c0
// name: FUN_004553c0
// ranges: 0x4553c0-0x455402
// calls: 0x4553c0

undefined4 __cdecl FUN_004553c0(uint param_1)

{
  undefined4 *puVar1;
  
  if ((param_1 < DAT_004a7710) &&
     (puVar1 = (undefined4 *)
               (*(int *)((int)&DAT_004a7720 + ((int)(param_1 & 0xffffffe7) >> 3)) +
               (param_1 & 0x1f) * 8), (*(byte *)(puVar1 + 1) & 1) != 0)) {
    return *puVar1;
  }
  DAT_00462c74 = 9;
  DAT_00462c78 = 0;
  return 0xffffffff;
}
