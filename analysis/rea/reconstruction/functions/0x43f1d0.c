// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x43f1d0
// name: FUN_0043f1d0
// ranges: 0x43f1d0-0x43f2e8
// calls: 0x43f1d0

uint __cdecl FUN_0043f1d0(int *param_1)

{
  int *piVar1;
  byte bVar2;
  undefined4 *puVar3;
  uint uVar4;
  int iVar5;
  byte *pbVar6;
  
  puVar3 = (undefined4 *)param_1[5];
  pbVar6 = (byte *)*puVar3;
  iVar5 = puVar3[1];
  while( true ) {
    if (iVar5 == 0) {
      uVar4 = (*(code *)puVar3[3])(param_1);
      if ((char)uVar4 == '\0') {
        return uVar4 & 0xffffff00;
      }
      pbVar6 = (byte *)*puVar3;
      iVar5 = puVar3[1];
    }
    bVar2 = *pbVar6;
    while( true ) {
      pbVar6 = pbVar6 + 1;
      iVar5 = iVar5 + -1;
      if (bVar2 == 0xff) break;
      *(int *)(param_1[100] + 0x58) = *(int *)(param_1[100] + 0x58) + 1;
      *puVar3 = pbVar6;
      puVar3[1] = iVar5;
      if (iVar5 == 0) {
        uVar4 = (*(code *)puVar3[3])(param_1);
        if ((char)uVar4 == '\0') {
          return uVar4 & 0xffffff00;
        }
        pbVar6 = (byte *)*puVar3;
        iVar5 = puVar3[1];
      }
      bVar2 = *pbVar6;
    }
    do {
      if (iVar5 == 0) {
        uVar4 = (*(code *)puVar3[3])(param_1);
        if ((char)uVar4 == '\0') {
          return uVar4 & 0xffffff00;
        }
        pbVar6 = (byte *)*puVar3;
        iVar5 = puVar3[1];
      }
      iVar5 = iVar5 + -1;
      uVar4 = (uint)*pbVar6;
      pbVar6 = pbVar6 + 1;
    } while (uVar4 == 0xff);
    if (uVar4 != 0) break;
    piVar1 = (int *)(param_1[100] + 0x58);
    *piVar1 = *piVar1 + 2;
    *puVar3 = pbVar6;
    puVar3[1] = iVar5;
  }
  if (*(int *)(param_1[100] + 0x58) != 0) {
    *(undefined4 *)(*param_1 + 0x14) = 0x70;
    *(undefined4 *)(*param_1 + 0x18) = *(undefined4 *)(param_1[100] + 0x58);
    *(uint *)(*param_1 + 0x1c) = uVar4;
    (**(code **)(*param_1 + 4))(param_1,0xffffffff);
    *(undefined4 *)(param_1[100] + 0x58) = 0;
  }
  param_1[0x5e] = uVar4;
  *puVar3 = pbVar6;
  puVar3[1] = iVar5;
  return 1;
}
