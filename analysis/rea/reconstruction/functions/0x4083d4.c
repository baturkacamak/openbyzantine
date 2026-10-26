// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4083d4
// name: FUN_004083d4
// ranges: 0x4083d4-0x4085e6
// calls: 0x407df9, 0x407e74, 0x407eef, 0x407f6a, 0x407fe5, 0x4083d4, 0x40de7b, 0x40df05

void FUN_004083d4(void)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int local_14;
  int local_10;
  int local_8;
  
  iVar1 = (int)DAT_00486a0a;
  iVar2 = (int)DAT_00486a0c;
  if (0x1a3 < iVar2) {
    if (DAT_0045d1c0 == 0) {
      FUN_0040df05(0);
    }
    else {
      local_8 = 0;
      if (DAT_0045d1c8 == 0) {
        iVar3 = FUN_00407df9();
        if (iVar3 == 0) {
          iVar3 = FUN_00407e74();
          if (iVar3 == 0) {
            local_10 = -1;
            for (local_14 = 0; local_14 < *(int *)(&DAT_00468cc0 + DAT_0045d1c4 * 4);
                local_14 = local_14 + 1) {
              if ((((*(int *)(&DAT_00468d00 + local_14 * 0x10) < iVar1) &&
                   (iVar1 < *(int *)(&DAT_00468d08 + local_14 * 0x10))) &&
                  (*(int *)(&DAT_00468d04 + local_14 * 0x10) < iVar2)) &&
                 (iVar2 < *(int *)(&DAT_00468d0c + local_14 * 0x10))) {
                local_10 = local_14;
                break;
              }
            }
            if ((local_10 != -1) &&
               (*(int *)(&DAT_00486f90 +
                        *(int *)(&DAT_00468d78 +
                                (DAT_0045d1bc + local_10) * 4 + DAT_0045d1c4 * 0x578) * 4) !=
                DAT_0045d1cc)) {
              local_8 = FUN_0040de7b(0x1c);
            }
          }
          else if (DAT_0045d1bc < *(int *)(&DAT_00468cc0 + DAT_0045d1c4 * 4) + -6) {
            local_8 = FUN_0040de7b(0x1c);
          }
        }
        else if (0 < DAT_0045d1bc) {
          local_8 = FUN_0040de7b(0x1c);
        }
      }
      else {
        iVar1 = FUN_00407eef();
        if (((iVar1 == 0) && (iVar1 = FUN_00407f6a(), iVar1 == 0)) ||
           (iVar1 = FUN_00407fe5(DAT_0045d1c8), iVar1 == 0)) {
          local_8 = *(int *)(DAT_0045d1c8 + 4);
        }
        else {
          local_8 = *(int *)(DAT_0045d1c8 + 4) + 1;
        }
      }
      FUN_0040df05((ushort)local_8);
      DAT_00483050 = local_8;
    }
  }
  return;
}
