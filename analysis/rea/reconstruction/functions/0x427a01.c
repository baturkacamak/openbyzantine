// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x427a01
// name: FUN_00427a01
// ranges: 0x427a01-0x427b5f
// calls: 0x422801, 0x427a01, 0x428143, 0x44ba00, 0x44c2f0

void __cdecl FUN_00427a01(uint param_1)

{
  int iVar1;
  uint *puVar2;
  int iVar3;
  uint *puVar4;
  uint *puVar5;
  uint *local_8;
  
  iVar3 = *(int *)(&DAT_00486220 + param_1 * 4);
  iVar1 = FUN_00428143(param_1);
  if (((iVar1 == 0) && (*(int *)(&DAT_00485c90 + param_1 * 4) != 0)) &&
     (*(int *)(iVar3 + 0x14) != 0)) {
    puVar2 = (uint *)FUN_0044c2f0(0x5c);
    puVar2[0x15] = *(uint *)(&DAT_00485c90 + param_1 * 4);
    puVar2[3] = param_1;
    puVar2[4] = (uint)DAT_00482428;
    puVar2[0x16] = 0;
    puVar2[5] = (int)*(char *)(*(int *)(&DAT_0047a8e0 + param_1 * 4) + 8);
    FUN_0044ba00(puVar2,(uint *)**(undefined4 **)(&DAT_0047a8e0 + param_1 * 4));
    puVar4 = *(uint **)(&DAT_00486220 + param_1 * 4);
    puVar5 = puVar2 + 6;
    for (iVar3 = 0xf; iVar3 != 0; iVar3 = iVar3 + -1) {
      *puVar5 = *puVar4;
      puVar4 = puVar4 + 1;
      puVar5 = puVar5 + 1;
    }
    puVar4 = puVar2;
    if (DAT_0046019c != (uint *)0x0) {
      for (local_8 = DAT_0046019c; local_8[0x16] != 0; local_8 = (uint *)local_8[0x16]) {
      }
      local_8[0x16] = (uint)puVar2;
      puVar4 = DAT_0046019c;
    }
    DAT_0046019c = puVar4;
    *(undefined4 *)(&DAT_00485c90 + param_1 * 4) = 0;
    *(undefined4 *)(*(int *)(&DAT_00486220 + param_1 * 4) + 0x34) = 0;
    if (puVar2[5] == 2) {
      FUN_00422801();
    }
  }
  return;
}
