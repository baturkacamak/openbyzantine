// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x459440
// name: FUN_00459440
// ranges: 0x459440-0x4594ba
// calls: 0x459190, 0x459440

void __cdecl FUN_00459440(int *param_1,uint param_2,int param_3)

{
  ushort *puVar1;
  uint uVar2;
  ushort *puVar3;
  uint uVar4;
  char *pcVar5;
  ushort local_c;
  undefined4 uStack_a;
  undefined2 uStack_6;
  undefined4 local_4;
  
  pcVar5 = s_GetActiveWindow_00463fbc + 0xc;
  if (param_2 != 0) {
    if ((int)param_2 < 0) {
      param_2 = -param_2;
      pcVar5 = &DAT_00464128;
    }
    if (param_3 == 0) {
      *(undefined2 *)param_1 = 0;
    }
    while (param_2 != 0) {
      pcVar5 = pcVar5 + 0x54;
      uVar4 = (int)param_2 >> 3;
      uVar2 = param_2 & 7;
      param_2 = uVar4;
      if (uVar2 != 0) {
        puVar1 = (ushort *)(pcVar5 + uVar2 * 0xc);
        puVar3 = puVar1;
        if (0x7fff < *puVar1) {
          puVar3 = &local_c;
          local_c = (ushort)*(undefined4 *)puVar1;
          uStack_a._0_2_ = (undefined2)((uint)*(undefined4 *)puVar1 >> 0x10);
          uStack_a._2_2_ = (undefined2)*(undefined4 *)(puVar1 + 2);
          uStack_6 = (undefined2)((uint)*(undefined4 *)(puVar1 + 2) >> 0x10);
          local_4 = *(undefined4 *)(puVar1 + 4);
          uStack_a = CONCAT22(uStack_a._2_2_,(undefined2)uStack_a) + -1;
        }
        FUN_00459190(param_1,(int *)puVar3);
      }
    }
  }
  return;
}
