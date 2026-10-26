// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4551c0
// name: FUN_004551c0
// ranges: 0x4551c0-0x455277
// calls: 0x44c2f0, 0x4551c0

int FUN_004551c0(void)

{
  undefined4 *puVar1;
  undefined4 *puVar2;
  int *piVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  
  iVar5 = -1;
  iVar6 = 0;
  iVar4 = 0;
  piVar3 = &DAT_004a7720;
  do {
    puVar1 = (undefined4 *)*piVar3;
    if (puVar1 == (undefined4 *)0x0) {
      puVar1 = (undefined4 *)FUN_0044c2f0(0x100);
      if (puVar1 != (undefined4 *)0x0) {
        DAT_004a7710 = DAT_004a7710 + 0x20;
        (&DAT_004a7720)[iVar6] = puVar1;
        if (puVar1 < puVar1 + 0x40) {
          do {
            *(undefined1 *)(puVar1 + 1) = 0;
            puVar2 = puVar1 + 2;
            *puVar1 = 0xffffffff;
            *(undefined1 *)((int)puVar1 + 5) = 10;
            puVar1 = puVar2;
          } while (puVar2 < (undefined4 *)((&DAT_004a7720)[iVar6] + 0x100));
        }
        iVar5 = iVar6 << 5;
      }
      return iVar5;
    }
    puVar2 = puVar1 + 0x40;
    for (; puVar1 < puVar2; puVar1 = puVar1 + 2) {
      if ((*(byte *)(puVar1 + 1) & 1) == 0) {
        *puVar1 = 0xffffffff;
        iVar5 = ((int)puVar1 - *piVar3 >> 3) + iVar4;
        break;
      }
    }
    if (iVar5 != -1) {
      return iVar5;
    }
    iVar4 = iVar4 + 0x20;
    piVar3 = piVar3 + 1;
    iVar6 = iVar6 + 1;
    if ((int *)0x4a781f < piVar3) {
      return -1;
    }
  } while( true );
}
