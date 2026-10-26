// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4429c0
// name: FUN_004429c0
// ranges: 0x4429c0-0x442a55
// calls: 0x4429c0

void __cdecl FUN_004429c0(int param_1)

{
  undefined4 *puVar1;
  int iVar2;
  int iVar3;
  uint uVar4;
  undefined4 *puVar5;
  uint uVar6;
  int iVar7;
  int iVar8;
  
  iVar7 = 0;
  iVar2 = *(int *)(param_1 + 0x180);
  iVar8 = *(int *)(param_1 + 0xc4);
  if (0 < *(int *)(param_1 + 0x20)) {
    do {
      uVar6 = *(int *)(iVar8 + 0x24) * *(int *)(iVar8 + 0xc);
      iVar3 = (int)uVar6 / *(int *)(param_1 + 0x114);
      uVar4 = *(uint *)(iVar8 + 0x2c) % uVar6;
      if (uVar4 == 0) {
        uVar4 = uVar6;
      }
      if (iVar7 == 0) {
        *(int *)(iVar2 + 0x48) = (int)(uVar4 - 1) / iVar3 + 1;
      }
      if (0 < iVar3 * 2) {
        puVar1 = (undefined4 *)
                 (*(int *)(*(int *)(iVar2 + 0x38 + *(int *)(iVar2 + 0x40) * 4) + iVar7 * 4) +
                 uVar4 * 4);
        iVar3 = iVar3 * 2;
        puVar5 = puVar1;
        do {
          iVar3 = iVar3 + -1;
          *puVar5 = puVar1[-1];
          puVar5 = puVar5 + 1;
        } while (iVar3 != 0);
      }
      iVar7 = iVar7 + 1;
      iVar8 = iVar8 + 0x54;
    } while (iVar7 < *(int *)(param_1 + 0x20));
  }
  return;
}
