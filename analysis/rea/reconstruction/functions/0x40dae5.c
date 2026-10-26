// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x40dae5
// name: FUN_0040dae5
// ranges: 0x40dae5-0x40dd7e
// calls: 0x40da4c, 0x40dae5, 0x40e89f, 0x40ed73, 0x416710, 0x44ac40, 0x44c2f0

void __cdecl FUN_0040dae5(int param_1)

{
  undefined4 uVar1;
  int iVar2;
  uint local_134;
  int local_130;
  int local_12c;
  uint local_128;
  char local_124 [260];
  int local_20;
  char *local_1c;
  undefined4 local_18 [4];
  int local_8;
  
  local_20 = 0;
  for (local_12c = 0; local_12c < 4; local_12c = local_12c + 1) {
    uVar1 = FUN_0044c2f0(0x105);
    local_18[local_12c] = uVar1;
  }
  if ((DAT_00476338 < 0x20) && (1 < *(int *)(PTR_PTR_0045e534 + (uint)DAT_00476338 * 0x24 + 0x10)))
  {
    FUN_0040ed73();
  }
  FUN_0040da4c();
  if (param_1 == -1) {
    PTR_PTR_0045e534 = (undefined *)&PTR_s_default_0045dbe8;
    local_1c = s_cursors__s__tga_0045e6ec;
  }
  else {
    PTR_PTR_0045e534 = (undefined *)&PTR_s_default_0045e090;
    local_1c = s_cursors_klio__s__tga_0045e6fc;
  }
  for (local_12c = 0; local_12c < 0x20; local_12c = local_12c + 1) {
    if (*(int *)(PTR_PTR_0045e534 + local_12c * 0x24) != 0) {
      _sprintf(local_124,local_1c,*(undefined4 *)(PTR_PTR_0045e534 + local_12c * 0x24));
      local_8 = FUN_0040e89f(local_124,(int)local_18,4);
      if (local_8 == 0) {
        local_20 = 1;
      }
      for (local_130 = 0; local_130 < local_8; local_130 = local_130 + 1) {
        iVar2 = FUN_00416710((char *)local_18[local_130],
                             (undefined4 *)
                             (PTR_PTR_0045e534 + local_12c * 0x24 + local_130 * 4 + 0x14),0,
                             &local_134,&local_128);
        local_20 = local_20 + iVar2;
        *(undefined4 *)(PTR_PTR_0045e534 + local_12c * 0x24 + 0xc) = 0;
      }
      *(int *)(PTR_PTR_0045e534 + local_12c * 0x24 + 0x10) = local_8;
    }
  }
  DAT_0046c45c = 0x30;
  DAT_0046c458 = 0x30;
  iVar2 = (**(code **)(*DAT_0045dbd4 + 0x1c))
                    (DAT_0045dbd4,0,0,*(undefined4 *)(PTR_PTR_0045e534 + 0x14),&DAT_0046c450,0x10);
  local_20 = local_20 + iVar2;
  DAT_00476338 = 0;
  for (local_12c = 0; local_12c < 4; local_12c = local_12c + 1) {
    FUN_0044ac40((LPVOID)local_18[local_12c]);
  }
  return;
}
