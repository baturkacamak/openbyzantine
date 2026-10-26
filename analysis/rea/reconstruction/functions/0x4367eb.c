// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4367eb
// name: FUN_004367eb
// ranges: 0x4367eb-0x43688a
// calls: 0x4367eb

void __cdecl FUN_004367eb(int param_1,int param_2,int *param_3,byte param_4)

{
  undefined1 local_8;
  
  if ((*(byte *)(param_4 + 1 + param_2) & 0x10) == 0) {
    *param_3 = *(int *)(param_1 + 0x58c + (uint)*(ushort *)(param_2 + 10 + (uint)param_4 * 4) * 4);
  }
  else {
    *param_3 = *(int *)(param_1 + 0x4c4 + (uint)*(ushort *)(param_2 + 10 + (uint)param_4 * 4) * 4);
  }
  for (local_8 = 0; (ushort)local_8 < *(ushort *)(param_2 + 0xc + (uint)param_4 * 4);
      local_8 = local_8 + 1) {
    *param_3 = *(int *)(*param_3 + 8);
  }
  return;
}
