// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x442250
// name: FUN_00442250
// ranges: 0x442250-0x442295
// calls: 0x442250

void __cdecl FUN_00442250(int param_1,int param_2,int param_3,int param_4,int param_5,uint param_6)

{
  uint uVar1;
  undefined4 *puVar2;
  undefined4 *puVar3;
  undefined4 *puVar4;
  undefined4 *puVar5;
  
  puVar2 = (undefined4 *)(param_1 + param_2 * 4);
  puVar3 = (undefined4 *)(param_3 + param_4 * 4);
  if (0 < param_5) {
    do {
      puVar4 = (undefined4 *)*puVar2;
      puVar5 = (undefined4 *)*puVar3;
      puVar2 = puVar2 + 1;
      puVar3 = puVar3 + 1;
      for (uVar1 = param_6 >> 2; uVar1 != 0; uVar1 = uVar1 - 1) {
        *puVar5 = *puVar4;
        puVar4 = puVar4 + 1;
        puVar5 = puVar5 + 1;
      }
      for (uVar1 = param_6 & 3; uVar1 != 0; uVar1 = uVar1 - 1) {
        *(undefined1 *)puVar5 = *(undefined1 *)puVar4;
        puVar4 = (undefined4 *)((int)puVar4 + 1);
        puVar5 = (undefined4 *)((int)puVar5 + 1);
      }
      param_5 = param_5 + -1;
    } while (param_5 != 0);
  }
  return;
}
