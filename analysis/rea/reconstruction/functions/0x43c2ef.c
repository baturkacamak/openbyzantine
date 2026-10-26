// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x43c2ef
// name: FUN_0043c2ef
// ranges: 0x43c2ef-0x43c3ab
// calls: 0x43c2ef

void __cdecl FUN_0043c2ef(ushort param_1)

{
  ushort uVar1;
  
  for (uVar1 = 0; uVar1 < param_1; uVar1 = uVar1 + 1) {
    if (((&DAT_0046caf8)[(uint)uVar1 * 4] != 0xffff) &&
       (((&DAT_0046cb00)[(uint)uVar1 * 4] == (&DAT_0046caf8)[(uint)uVar1 * 4] ||
        ((&DAT_0046cafc)[(uint)uVar1 * 4] == *(int *)(&DAT_0046cb04 + (uint)uVar1 * 0x10))))) {
      (&DAT_0046caf8)[(uint)uVar1 * 4] = 0xffff;
    }
  }
  return;
}
