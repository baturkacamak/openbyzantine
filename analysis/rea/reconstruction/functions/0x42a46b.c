// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x42a46b
// name: FUN_0042a46b
// ranges: 0x42a46b-0x42a554
// calls: 0x42a46b, 0x42a900, 0x42a970

void __cdecl FUN_0042a46b(int *param_1,int *param_2)

{
  uint uVar1;
  int iVar2;
  int local_c;
  int local_8;
  
  local_8 = 0;
  if (*param_2 < 0) {
    *param_1 = -1;
  }
  local_c = 0;
  do {
    if (0x17 < local_c) {
      *param_2 = local_8;
      return;
    }
    uVar1 = FUN_0042a900((&DAT_0046ca98)[local_c]);
    if (uVar1 == 0) {
      iVar2 = FUN_0042a970((&DAT_0046ca98)[local_c]);
      if (*param_1 < iVar2) {
        iVar2 = FUN_0042a970((&DAT_0046ca98)[local_c]);
        *param_1 = iVar2;
        local_8 = 1;
      }
      else {
        iVar2 = FUN_0042a970((&DAT_0046ca98)[local_c]);
        if (iVar2 == *param_1) {
          local_8 = local_8 + 1;
        }
      }
      if ((-1 < *param_2) && (*param_2 == local_8)) {
        *param_2 = local_c;
        return;
      }
    }
    local_c = local_c + 1;
  } while( true );
}
