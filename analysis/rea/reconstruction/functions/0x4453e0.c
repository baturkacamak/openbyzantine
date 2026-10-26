// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4453e0
// name: FUN_004453e0
// ranges: 0x4453e0-0x4455dc
// calls: 0x4453e0

void __cdecl FUN_004453e0(int *param_1)

{
  ushort uVar1;
  float fVar2;
  int iVar3;
  short *psVar4;
  short *psVar5;
  double *pdVar6;
  ushort *puVar7;
  ushort *puVar8;
  uint *puVar9;
  int *piVar10;
  float *pfVar11;
  float *pfVar12;
  double *pdVar13;
  int iVar14;
  int local_18;
  undefined1 *local_14;
  int *local_10;
  int local_c;
  
  iVar14 = 0;
  local_18 = param_1[0x31];
  local_14 = (undefined1 *)0x0;
  local_c = 0;
  if (0 < param_1[8]) {
    local_10 = (int *)(param_1[0x66] + 0x2c);
    do {
      switch(*(undefined4 *)(local_18 + 0x24)) {
      case 1:
        local_14 = &LAB_0044a270;
        iVar14 = 0;
        break;
      case 2:
        local_14 = &LAB_0044a020;
        iVar14 = 0;
        break;
      default:
        *(undefined4 *)(*param_1 + 0x14) = 6;
        *(undefined4 *)(*param_1 + 0x18) = *(undefined4 *)(local_18 + 0x24);
        (**(code **)*param_1)(param_1);
        break;
      case 4:
        local_14 = &LAB_00449bc0;
        iVar14 = 0;
        break;
      case 8:
        iVar3 = param_1[0x11];
        if (iVar3 == 0) {
          local_14 = &LAB_004494b0;
          iVar14 = 0;
        }
        else if (iVar3 == 1) {
          local_14 = &LAB_00449010;
          iVar14 = 1;
        }
        else if (iVar3 == 2) {
          local_14 = &LAB_00448bb0;
          iVar14 = 2;
        }
        else {
          *(undefined4 *)(*param_1 + 0x14) = 0x2f;
          (**(code **)*param_1)(param_1);
        }
      }
      local_10[-10] = (int)local_14;
      if (((*(char *)(local_18 + 0x30) != '\0') && (*local_10 != iVar14)) &&
         (puVar7 = *(ushort **)(local_18 + 0x4c), puVar7 != (ushort *)0x0)) {
        *local_10 = iVar14;
        if (iVar14 == 0) {
          iVar3 = 0x40;
          puVar9 = *(uint **)(local_18 + 0x50);
          do {
            iVar3 = iVar3 + -1;
            *puVar9 = (uint)*puVar7;
            puVar7 = puVar7 + 1;
            puVar9 = puVar9 + 1;
          } while (iVar3 != 0);
        }
        else if (iVar14 == 1) {
          psVar4 = &DAT_0045b5f8;
          piVar10 = *(int **)(local_18 + 0x50);
          do {
            psVar5 = psVar4 + 1;
            uVar1 = *puVar7;
            puVar7 = puVar7 + 1;
            *piVar10 = (int)((uint)uVar1 * (int)*psVar4 + 0x800) >> 0xc;
            psVar4 = psVar5;
            piVar10 = piVar10 + 1;
          } while (psVar5 < &DAT_0045b678);
        }
        else if (iVar14 == 2) {
          pdVar6 = (double *)&DAT_0045b678;
          pfVar11 = *(float **)(local_18 + 0x50);
          do {
            puVar8 = puVar7;
            pfVar12 = pfVar11;
            pdVar13 = (double *)&DAT_0045b678;
            while( true ) {
              puVar7 = puVar8 + 1;
              pfVar11 = pfVar12 + 1;
              fVar2 = (float)*puVar8 * (float)*pdVar13 * (float)*pdVar6;
              if (&UNK_0045b6b7 < pdVar13 + 1) break;
              *pfVar12 = fVar2;
              puVar8 = puVar7;
              pfVar12 = pfVar11;
              pdVar13 = pdVar13 + 1;
            }
            *pfVar12 = fVar2;
            pdVar6 = pdVar6 + 1;
          } while (pdVar6 < &DAT_0045b6b8);
        }
        else {
          *(undefined4 *)(*param_1 + 0x14) = 0x2f;
          (**(code **)*param_1)(param_1);
        }
      }
      local_10 = local_10 + 1;
      local_18 = local_18 + 0x54;
      local_c = local_c + 1;
    } while (local_c < param_1[8]);
  }
  return;
}
