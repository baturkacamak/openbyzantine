// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x446080
// name: FUN_00446080
// ranges: 0x446080-0x4461c9
// calls: 0x446080, 0x4461e0

void __cdecl FUN_00446080(int *param_1)

{
  undefined4 *puVar1;
  int iVar2;
  int iVar3;
  int *piVar4;
  
  puVar1 = (undefined4 *)(**(code **)param_1[1])(param_1,1,0x18);
  param_1[0x68] = (int)puVar1;
  *puVar1 = &DAT_004465b0;
  switch(param_1[9]) {
  case 1:
    if (param_1[8] != 1) {
LAB_004460d8:
      *(undefined4 *)(*param_1 + 0x14) = 8;
      (**(code **)*param_1)(param_1);
    }
    break;
  case 2:
  case 3:
    if (param_1[8] != 3) goto LAB_004460d8;
    break;
  case 4:
  case 5:
    if (param_1[8] != 4) goto LAB_004460d8;
    break;
  default:
    if (param_1[8] < 1) goto LAB_004460d8;
  }
  piVar4 = param_1 + 8;
  iVar2 = param_1[10];
  if (iVar2 == 1) {
    param_1[0x19] = 1;
    if ((param_1[9] == 1) || (param_1[9] == 3)) {
      puVar1[1] = &LAB_00446430;
      iVar2 = 1;
      if (1 < *piVar4) {
        iVar3 = 0x54;
        do {
          iVar3 = iVar3 + 0x54;
          iVar2 = iVar2 + 1;
          *(undefined1 *)(param_1[0x31] + -0x24 + iVar3) = 0;
        } while (iVar2 < *piVar4);
      }
      goto LAB_004461af;
    }
  }
  else if (iVar2 == 2) {
    param_1[0x19] = 3;
    if (param_1[9] == 3) {
      puVar1[1] = &LAB_004462b0;
      FUN_004461e0((int)param_1);
      goto LAB_004461af;
    }
    if (param_1[9] == 2) goto LAB_00446195;
  }
  else if (iVar2 == 4) {
    param_1[0x19] = 4;
    if (param_1[9] == 5) {
      puVar1[1] = &LAB_00446460;
      FUN_004461e0((int)param_1);
      goto LAB_004461af;
    }
    if (param_1[9] == 4) goto LAB_00446195;
  }
  else if (param_1[9] == iVar2) {
    param_1[0x19] = *piVar4;
LAB_00446195:
    puVar1[1] = &LAB_004463c0;
    goto LAB_004461af;
  }
  *(undefined4 *)(*param_1 + 0x14) = 0x19;
  (**(code **)*param_1)(param_1);
LAB_004461af:
  if (*(char *)((int)param_1 + 0x4a) == '\0') {
    param_1[0x1a] = param_1[0x19];
    return;
  }
  param_1[0x1a] = 1;
  return;
}
