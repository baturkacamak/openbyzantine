// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4077b2
// name: FUN_004077b2
// ranges: 0x4077b2-0x4078f3
// calls: 0x407254, 0x407420, 0x4077b2, 0x407b9c

void __cdecl FUN_004077b2(uint param_1,uint param_2)

{
  char *pcVar1;
  int iVar2;
  int local_10;
  
  pcVar1 = *(char **)(&DAT_00486f90 + (param_2 & 0xffff) * 4);
  if ((pcVar1 != (char *)0x0) && (*pcVar1 != '\x01')) {
    *pcVar1 = '\x01';
    if (pcVar1 == DAT_0045d1c8) {
      FUN_00407b9c(0);
    }
    else {
      iVar2 = FUN_00407254((int)pcVar1);
      if (iVar2 == 0) {
        local_10 = *(int *)(&DAT_00468cc0 + (param_1 & 0xffff) * 4);
        *(int *)(&DAT_00468cc0 + (param_1 & 0xffff) * 4) =
             *(int *)(&DAT_00468cc0 + (param_1 & 0xffff) * 4) + 1;
        for (; 0 < local_10; local_10 = local_10 + -1) {
          *(undefined4 *)(&DAT_00468d78 + (param_1 & 0xffff) * 0x578 + local_10 * 4) =
               *(undefined4 *)(&DAT_00468d74 + local_10 * 4 + (param_1 & 0xffff) * 0x578);
        }
        *(uint *)(&DAT_00468d78 + (param_1 & 0xffff) * 0x578) = param_2 & 0xffff;
      }
    }
    if ((param_1 & 0xffff) == DAT_0045d1c4) {
      FUN_00407420(1);
    }
  }
  return;
}
