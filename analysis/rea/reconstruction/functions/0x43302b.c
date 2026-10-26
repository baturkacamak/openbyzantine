// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x43302b
// name: FUN_0043302b
// ranges: 0x43302b-0x4330be
// calls: 0x43302b

ushort __cdecl FUN_0043302b(int param_1,byte param_2)

{
  ushort uVar1;
  
  uVar1 = (ushort)param_2;
  if (param_2 == 0xff) {
    for (uVar1 = 0; (*(int *)(param_1 + 0x4c4 + (uint)uVar1 * 4) == 0 && (uVar1 < 0x32));
        uVar1 = uVar1 + 1) {
    }
    if (uVar1 == 0x32) {
      uVar1 = *(ushort *)(param_1 + 8);
    }
  }
  return uVar1;
}
