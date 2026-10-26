// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x42a579
// name: FUN_0042a579
// ranges: 0x42a579-0x42a5bd; 0x42a5c3-0x42a63a; 0x42a640-0x42a6a9
// calls: 0x42a005, 0x42a10e, 0x42a217, 0x42a349, 0x42a46b, 0x42a555, 0x42a579, 0x42a920, 0x42a950

undefined4 __cdecl FUN_0042a579(uint param_1,uint *param_2,undefined4 *param_3)

{
  int iVar1;
  uint uVar2;
  int local_14;
  undefined4 local_10;
  uint local_c;
  int local_8;
  
  local_10 = 0;
  local_c = param_1;
  CPropertySheet::SetWizardMode((CPropertySheet *)(&DAT_0046ca98)[param_1]);
  iVar1 = FUN_0042a10e(param_3);
  if (iVar1 == 0) {
    local_8 = FUN_0042a217();
    if (local_8 < 0) {
      uVar2 = FUN_0042a950((&DAT_0046ca98)[local_c]);
      if (uVar2 != 0) {
        FUN_0042a349(local_c);
      }
    }
    else {
      FUN_0042a920((void *)(&DAT_0046ca98)[local_8],0x14);
    }
    local_c = 0xffffffff;
    FUN_0042a46b(&local_14,(int *)&local_c);
    if (local_14 == -1) {
      local_10 = 3;
    }
    else {
      local_c = FUN_0042a555(1,local_c);
      FUN_0042a46b(&local_14,(int *)&local_c);
      CPropertySheet::SetWizardMode((CPropertySheet *)(&DAT_0046ca98)[local_c]);
      *param_2 = local_c;
      iVar1 = FUN_0042a005(param_3);
      if (iVar1 != 0) {
        local_10 = 1;
      }
    }
  }
  else {
    local_10 = 2;
  }
  return local_10;
}
