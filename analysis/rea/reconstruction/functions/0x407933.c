// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x407933
// name: FUN_00407933
// ranges: 0x407933-0x407ad4
// calls: 0x407420, 0x407933, 0x407b9c

void __cdecl FUN_00407933(uint param_1,uint param_2)

{
  char *pcVar1;
  int local_10;
  int local_c;
  
  pcVar1 = *(char **)(&DAT_00486f90 + (param_2 & 0xffff) * 4);
  if (((pcVar1 == (char *)0x0) || (*pcVar1 == '\0')) || (*pcVar1 == '\x02')) {
    return;
  }
  if (*pcVar1 == '\x03') {
    FUN_00407b9c(0);
  }
  local_c = 0;
  while( true ) {
    if (*(int *)(&DAT_00468cc0 + (param_1 & 0xffff) * 4) <= local_c) goto LAB_00407a81;
    if (*(uint *)(&DAT_00468d78 + (param_1 & 0xffff) * 0x578 + local_c * 4) == (param_2 & 0xffff))
    break;
    local_c = local_c + 1;
  }
  for (local_10 = local_c; local_10 < *(int *)(&DAT_00468cc0 + (param_1 & 0xffff) * 4) + -1;
      local_10 = local_10 + 1) {
    *(undefined4 *)(&DAT_00468d78 + (param_1 & 0xffff) * 0x578 + local_10 * 4) =
         *(undefined4 *)(local_10 * 4 + 0x468d7c + (param_1 & 0xffff) * 0x578);
  }
  *(int *)(&DAT_00468cc0 + (param_1 & 0xffff) * 4) =
       *(int *)(&DAT_00468cc0 + (param_1 & 0xffff) * 4) + -1;
LAB_00407a81:
  if ((DAT_0045d1bc != 0) && (*(int *)(&DAT_00468cc0 + (param_1 & 0xffff) * 4) < DAT_0045d1bc + 6))
  {
    DAT_0045d1bc = DAT_0045d1bc + -1;
  }
  if ((param_1 & 0xffff) != DAT_0045d1c4) {
    return;
  }
  FUN_00407420(1);
  return;
}
