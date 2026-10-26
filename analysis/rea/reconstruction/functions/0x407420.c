// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x407420
// name: FUN_00407420
// ranges: 0x407420-0x407526
// calls: 0x407254, 0x407420, 0x4075fe, 0x4076b4

void __cdecl FUN_00407420(int param_1)

{
  int iVar1;
  int local_18;
  short local_c;
  int local_8;
  
  local_8 = DAT_0045d1bc;
  local_c = 0xa0;
  FUN_004076b4(DAT_0045d1b4,0,0);
  for (local_18 = 0; (local_18 < 6 && (*(int *)(&DAT_00468cc0 + DAT_0045d1c4 * 4) != local_18));
      local_18 = local_18 + 1) {
    iVar1 = local_8 * 4;
    local_8 = local_8 + 1;
    iVar1 = *(int *)(&DAT_00486f90 + *(int *)(&DAT_00468d78 + iVar1 + DAT_0045d1c4 * 0x578) * 4);
    if ((iVar1 != DAT_0045d1c8) && (iVar1 != DAT_0045d1cc)) {
      if (*(int *)(iVar1 + 0xc) == 0) {
        FUN_00407254(iVar1);
      }
      FUN_004076b4(*(int **)(iVar1 + 0xc),local_c,6);
    }
    local_c = local_c + (short)DAT_0045d1ac;
  }
  FUN_004075fe(param_1);
  return;
}
