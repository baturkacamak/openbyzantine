// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x444ad0
// name: FUN_00444ad0
// ranges: 0x444ad0-0x444b42
// calls: 0x444ad0

uint __cdecl FUN_00444ad0(int param_1)

{
  int *piVar1;
  int iVar2;
  undefined4 uVar3;
  uint uVar4;
  undefined4 *puVar5;
  int iVar6;
  
  iVar2 = *(int *)(param_1 + 0x194);
  piVar1 = (int *)(*(int *)(param_1 + 400) + 0x58);
  *piVar1 = *piVar1 + ((int)(*(int *)(iVar2 + 0xc) + (*(int *)(iVar2 + 0xc) >> 0x1f & 7U)) >> 3);
  *(undefined4 *)(iVar2 + 0xc) = 0;
  uVar4 = (**(code **)(*(int *)(param_1 + 400) + 8))(param_1);
  if ((char)uVar4 == '\0') {
    return uVar4 & 0xffffff00;
  }
  iVar6 = 0;
  if (0 < *(int *)(param_1 + 0x120)) {
    puVar5 = (undefined4 *)(iVar2 + 0x18);
    do {
      *puVar5 = 0;
      puVar5 = puVar5 + 1;
      iVar6 = iVar6 + 1;
    } while (iVar6 < *(int *)(param_1 + 0x120));
  }
  *(undefined4 *)(iVar2 + 0x14) = 0;
  uVar3 = *(undefined4 *)(param_1 + 0xfc);
  *(undefined4 *)(iVar2 + 0x28) = uVar3;
  *(undefined1 *)(iVar2 + 0x10) = 0;
  return CONCAT31((int3)((uint)uVar3 >> 8),1);
}
