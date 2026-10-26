// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x448170
// name: FUN_00448170
// ranges: 0x448170-0x44824d
// calls: 0x448170

int __cdecl FUN_00448170(int *param_1,int *param_2)

{
  int iVar1;
  int iVar2;
  bool bVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int *piVar8;
  int local_8;
  
  iVar1 = param_1[0x19];
  iVar2 = param_1[0x15];
  iVar7 = 1;
  do {
    iVar5 = iVar7;
    iVar7 = iVar5 + 1;
    iVar6 = iVar7;
    if (1 < iVar1) {
      iVar4 = iVar1 + -1;
      do {
        iVar6 = iVar6 * iVar7;
        iVar4 = iVar4 + -1;
      } while (iVar4 != 0);
    }
  } while (iVar6 <= iVar2);
  if (iVar5 < 2) {
    *(undefined4 *)(*param_1 + 0x14) = 0x37;
    *(int *)(*param_1 + 0x18) = iVar6;
    (**(code **)*param_1)(param_1);
  }
  local_8 = 1;
  iVar7 = iVar1;
  piVar8 = param_2;
  if (0 < iVar1) {
    for (; iVar6 = iVar1, iVar7 != 0; iVar7 = iVar7 + -1) {
      *piVar8 = iVar5;
      piVar8 = piVar8 + 1;
    }
    do {
      local_8 = iVar5 * local_8;
      iVar6 = iVar6 + -1;
    } while (iVar6 != 0);
  }
  do {
    iVar7 = 0;
    bVar3 = false;
    if (0 < iVar1) {
      do {
        iVar6 = iVar7;
        if (param_1[10] == 2) {
          iVar6 = (&DAT_0045b7b8)[iVar7];
        }
        iVar5 = (local_8 / param_2[iVar6]) * (param_2[iVar6] + 1);
        if (iVar2 < iVar5) break;
        iVar7 = iVar7 + 1;
        param_2[iVar6] = param_2[iVar6] + 1;
        bVar3 = true;
        local_8 = iVar5;
      } while (iVar7 < iVar1);
    }
    if (!bVar3) {
      return local_8;
    }
  } while( true );
}
