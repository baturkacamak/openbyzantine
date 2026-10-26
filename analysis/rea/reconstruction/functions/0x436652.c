// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x436652
// name: FUN_00436652
// ranges: 0x436652-0x436699
// calls: 0x436652

short __cdecl FUN_00436652(int param_1,byte param_2)

{
  return *(short *)(param_1 + 0xc + (uint)param_2 * 4) * 0x100 +
         *(short *)(param_1 + 10 + (uint)param_2 * 4);
}
