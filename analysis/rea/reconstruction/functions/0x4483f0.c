// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4483f0
// name: FUN_004483f0
// ranges: 0x4483f0-0x44842f
// calls: 0x4483f0

void __cdecl FUN_004483f0(int param_1)

{
  int iVar1;
  undefined4 uVar2;
  int iVar3;
  undefined4 *puVar4;
  
  iVar1 = *(int *)(param_1 + 0x5c);
  iVar3 = 0;
  if (0 < *(int *)(param_1 + 100)) {
    puVar4 = (undefined4 *)(*(int *)(param_1 + 0x1a4) + 0x44);
    do {
      iVar3 = iVar3 + 1;
      uVar2 = (**(code **)(*(int *)(param_1 + 4) + 4))(param_1,1,iVar1 * 2 + 4);
      *puVar4 = uVar2;
      puVar4 = puVar4 + 1;
    } while (iVar3 < *(int *)(param_1 + 100));
  }
  return;
}
