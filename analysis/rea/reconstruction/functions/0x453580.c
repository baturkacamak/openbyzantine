// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x453580
// name: FUN_00453580
// ranges: 0x453580-0x45365a
// calls: 0x44ac40, 0x44c2f0, 0x453580

void FUN_00453580(void)

{
  char cVar1;
  int *piVar2;
  uint uVar3;
  uint uVar4;
  uint uVar5;
  char *pcVar6;
  int iVar7;
  char *pcVar8;
  char *pcVar9;
  
  iVar7 = 0;
  cVar1 = *DAT_00463184;
  pcVar6 = DAT_00463184;
  while (cVar1 != '\0') {
    if (*pcVar6 != '=') {
      iVar7 = iVar7 + 1;
    }
    uVar3 = 0xffffffff;
    pcVar9 = pcVar6;
    do {
      if (uVar3 == 0) break;
      uVar3 = uVar3 - 1;
      cVar1 = *pcVar9;
      pcVar9 = pcVar9 + 1;
    } while (cVar1 != '\0');
    pcVar6 = pcVar6 + ~uVar3;
    cVar1 = *pcVar6;
  }
  piVar2 = (int *)FUN_0044c2f0(iVar7 * 4 + 4);
  DAT_00462c9c = piVar2;
  if (piVar2 == (int *)0x0) {
    __amsg_exit(9);
  }
  cVar1 = *DAT_00463184;
  pcVar6 = DAT_00463184;
  do {
    if (cVar1 == '\0') {
      FUN_0044ac40(DAT_00463184);
      *piVar2 = 0;
      return;
    }
    uVar3 = 0xffffffff;
    pcVar9 = pcVar6;
    do {
      if (uVar3 == 0) break;
      uVar3 = uVar3 - 1;
      cVar1 = *pcVar9;
      pcVar9 = pcVar9 + 1;
    } while (cVar1 != '\0');
    if (*pcVar6 != '=') {
      iVar7 = FUN_0044c2f0(~uVar3);
      *piVar2 = iVar7;
      if (iVar7 == 0) {
        __amsg_exit(9);
      }
      uVar4 = 0xffffffff;
      pcVar9 = pcVar6;
      do {
        pcVar8 = pcVar9;
        if (uVar4 == 0) break;
        uVar4 = uVar4 - 1;
        pcVar8 = pcVar9 + 1;
        cVar1 = *pcVar9;
        pcVar9 = pcVar8;
      } while (cVar1 != '\0');
      uVar4 = ~uVar4;
      pcVar9 = (char *)*piVar2;
      piVar2 = piVar2 + 1;
      pcVar8 = pcVar8 + -uVar4;
      for (uVar5 = uVar4 >> 2; uVar5 != 0; uVar5 = uVar5 - 1) {
        *(undefined4 *)pcVar9 = *(undefined4 *)pcVar8;
        pcVar8 = pcVar8 + 4;
        pcVar9 = pcVar9 + 4;
      }
      for (uVar4 = uVar4 & 3; uVar4 != 0; uVar4 = uVar4 - 1) {
        *pcVar9 = *pcVar8;
        pcVar8 = pcVar8 + 1;
        pcVar9 = pcVar9 + 1;
      }
    }
    pcVar6 = pcVar6 + ~uVar3;
    cVar1 = *pcVar6;
  } while( true );
}
