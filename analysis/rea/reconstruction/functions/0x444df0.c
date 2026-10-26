// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x444df0
// name: FUN_00444df0
// ranges: 0x444df0-0x444f00
// calls: 0x443c50, 0x444ad0, 0x444df0

uint __cdecl FUN_00444df0(int param_1,undefined4 *param_2)

{
  byte bVar1;
  int iVar2;
  ushort *puVar3;
  uint uVar4;
  uint uVar5;
  int iVar6;
  int iVar7;
  ushort local_20;
  undefined4 local_1c;
  undefined4 local_18;
  undefined4 local_14;
  uint local_10;
  int local_c;
  int local_8;
  int local_4;
  
  iVar2 = *(int *)(param_1 + 0x194);
  bVar1 = *(byte *)(param_1 + 0x174);
  if (((*(int *)(param_1 + 0xfc) != 0) && (*(int *)(iVar2 + 0x28) == 0)) &&
     (uVar4 = FUN_00444ad0(param_1), (char)uVar4 == '\0')) {
    return uVar4 & 0xffffff00;
  }
  local_8 = param_1;
  local_1c = **(undefined4 **)(param_1 + 0x14);
  local_18 = (*(undefined4 **)(param_1 + 0x14))[1];
  local_14 = *(undefined4 *)(param_1 + 0x178);
  uVar4 = *(uint *)(iVar2 + 8);
  iVar6 = *(int *)(iVar2 + 0xc);
  local_4 = iVar2 + 0x10;
  iVar7 = 0;
  if (0 < *(int *)(param_1 + 0x13c)) {
    do {
      puVar3 = (ushort *)*param_2;
      if ((iVar6 < 1) &&
         (uVar5 = FUN_00443c50(&local_1c,uVar4,iVar6,1), iVar6 = local_c, uVar4 = local_10,
         (char)uVar5 == '\0')) {
        return uVar5 & 0xffffff00;
      }
      iVar6 = iVar6 + -1;
      if (((int)uVar4 >> ((byte)iVar6 & 0x1f) & 1U) != 0) {
        local_20 = (ushort)(1 << (bVar1 & 0x1f));
        *puVar3 = *puVar3 | local_20;
      }
      param_2 = param_2 + 1;
      iVar7 = iVar7 + 1;
    } while (iVar7 < *(int *)(param_1 + 0x13c));
  }
  **(undefined4 **)(param_1 + 0x14) = local_1c;
  *(undefined4 *)(*(int *)(param_1 + 0x14) + 4) = local_18;
  *(undefined4 *)(param_1 + 0x178) = local_14;
  *(uint *)(iVar2 + 8) = uVar4;
  *(int *)(iVar2 + 0xc) = iVar6;
  *(int *)(iVar2 + 0x28) = *(int *)(iVar2 + 0x28) + -1;
  return CONCAT31((int3)((uint)local_14 >> 8),1);
}
