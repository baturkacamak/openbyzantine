// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4444c0
// name: FUN_004444c0
// ranges: 0x4444c0-0x44452e
// calls: 0x4444c0

uint __cdecl FUN_004444c0(int param_1)

{
  int *piVar1;
  int iVar2;
  undefined4 uVar3;
  uint uVar4;
  int iVar5;
  undefined4 *puVar6;
  
  iVar2 = *(int *)(param_1 + 0x194);
  piVar1 = (int *)(*(int *)(param_1 + 400) + 0x58);
  *piVar1 = *piVar1 + ((int)(*(int *)(iVar2 + 0xc) + (*(int *)(iVar2 + 0xc) >> 0x1f & 7U)) >> 3);
  *(undefined4 *)(iVar2 + 0xc) = 0;
  uVar4 = (**(code **)(*(int *)(param_1 + 400) + 8))(param_1);
  if ((char)uVar4 == '\0') {
    return uVar4 & 0xffffff00;
  }
  iVar5 = 0;
  if (0 < *(int *)(param_1 + 0x120)) {
    puVar6 = (undefined4 *)(iVar2 + 0x14);
    do {
      *puVar6 = 0;
      puVar6 = puVar6 + 1;
      iVar5 = iVar5 + 1;
    } while (iVar5 < *(int *)(param_1 + 0x120));
  }
  uVar3 = *(undefined4 *)(param_1 + 0xfc);
  *(undefined4 *)(iVar2 + 0x24) = uVar3;
  *(undefined1 *)(iVar2 + 0x10) = 0;
  return CONCAT31((int3)((uint)uVar3 >> 8),1);
}
