// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x408076
// name: FUN_00408076
// ranges: 0x408076-0x4083d3
// calls: 0x407420, 0x407527, 0x407ad5, 0x407b9c, 0x407df9, 0x407e74, 0x407eef, 0x407f6a, 0x407fe5, 0x408076

undefined4 FUN_00408076(void)

{
  char *pcVar1;
  bool bVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int local_1c;
  int local_18;
  undefined4 local_c;
  
  local_c = 0;
  iVar3 = (int)DAT_00486a0a;
  iVar4 = (int)DAT_00486a0c;
  iVar5 = FUN_00407f6a();
  if (iVar5 == 0) {
    iVar5 = FUN_00407eef();
    if (iVar5 == 0) {
      iVar5 = FUN_00407df9();
      if (iVar5 == 0) {
        iVar5 = FUN_00407e74();
        if (iVar5 == 0) {
          local_18 = -1;
          for (local_1c = 0; local_1c < *(int *)(&DAT_00468cc0 + DAT_0045d1c4 * 4);
              local_1c = local_1c + 1) {
            if ((((*(int *)(&DAT_00468d00 + local_1c * 0x10) < iVar3) &&
                 (iVar3 < *(int *)(&DAT_00468d08 + local_1c * 0x10))) &&
                (*(int *)(&DAT_00468d04 + local_1c * 0x10) < iVar4)) &&
               (iVar4 < *(int *)(&DAT_00468d0c + local_1c * 0x10))) {
              local_18 = local_1c;
              break;
            }
          }
          if (local_18 != -1) {
            iVar3 = *(int *)(&DAT_00468d78 + (DAT_0045d1bc + local_18) * 4 + DAT_0045d1c4 * 0x578);
            pcVar1 = *(char **)(&DAT_00486f90 + iVar3 * 4);
            if (*pcVar1 == '\x03') {
              *pcVar1 = '\x01';
              FUN_00407b9c(0);
              FUN_00407420(1);
            }
            else if (pcVar1 != DAT_0045d1cc) {
              if (DAT_0045d1c8 == (char *)0x0) {
                FUN_00407ad5(pcVar1);
              }
              else {
                bVar2 = false;
                local_1c = 0;
                while ((local_1c < *(int *)(&DAT_00468cc0 + DAT_0045d1c4 * 4) && (!bVar2))) {
                  if (**(char **)(&DAT_00486f90 +
                                 *(int *)(&DAT_00468d78 + local_1c * 4 + DAT_0045d1c4 * 0x578) * 4)
                      == '\x03') {
                    **(undefined1 **)
                      (&DAT_00486f90 +
                      *(int *)(&DAT_00468d78 + local_1c * 4 + DAT_0045d1c4 * 0x578) * 4) = 1;
                    *(undefined4 *)
                     (&DAT_00468d78 + (DAT_0045d1bc + local_18) * 4 + DAT_0045d1c4 * 0x578) =
                         *(undefined4 *)(&DAT_00468d78 + local_1c * 4 + DAT_0045d1c4 * 0x578);
                    *(int *)(&DAT_00468d78 + local_1c * 4 + DAT_0045d1c4 * 0x578) = iVar3;
                    bVar2 = true;
                  }
                  local_1c = local_1c + 1;
                }
                if (bVar2) {
                  FUN_00407ad5(pcVar1);
                }
              }
            }
          }
        }
        else {
          FUN_00407527(1);
        }
      }
      else {
        FUN_00407527(0);
      }
    }
    else {
      iVar3 = FUN_00407fe5((int)DAT_0045d1c8);
      if (iVar3 != 0) {
        *DAT_0045d1c8 = '\x01';
        FUN_00407b9c(0);
        FUN_00407420(1);
      }
    }
  }
  else {
    iVar3 = FUN_00407fe5((int)DAT_0045d1c8);
    if (iVar3 != 0) {
      if (DAT_0045d1cc != (char *)0x0) {
        *DAT_0045d1cc = '\x01';
      }
      DAT_0045d1cc = DAT_0045d1c8;
      *DAT_0045d1c8 = '\x04';
      FUN_00407b9c(0);
      FUN_00407420(0);
      local_c = 1;
    }
  }
  return local_c;
}
