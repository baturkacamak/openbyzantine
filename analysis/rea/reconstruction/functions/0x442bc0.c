// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x442bc0
// name: FUN_00442bc0
// ranges: 0x442bc0-0x442c15
// calls: 0x442bc0

void __cdecl FUN_00442bc0(int param_1)

{
  int iVar1;
  
  iVar1 = *(int *)(param_1 + 0x184);
  if (1 < *(int *)(param_1 + 0x120)) {
    *(undefined4 *)(iVar1 + 0x1c) = 1;
    *(undefined4 *)(iVar1 + 0x14) = 0;
    *(undefined4 *)(iVar1 + 0x18) = 0;
    return;
  }
  if (*(uint *)(param_1 + 0x80) < *(int *)(param_1 + 0x118) - 1U) {
    *(undefined4 *)(iVar1 + 0x1c) = *(undefined4 *)(*(int *)(param_1 + 0x124) + 0xc);
    *(undefined4 *)(iVar1 + 0x14) = 0;
    *(undefined4 *)(iVar1 + 0x18) = 0;
    return;
  }
  *(undefined4 *)(iVar1 + 0x1c) = *(undefined4 *)(*(int *)(param_1 + 0x124) + 0x48);
  *(undefined4 *)(iVar1 + 0x14) = 0;
  *(undefined4 *)(iVar1 + 0x18) = 0;
  return;
}
