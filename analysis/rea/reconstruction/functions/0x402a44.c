// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x402a44
// name: FUN_00402a44
// ranges: 0x402a44-0x402ba1
// calls: 0x402a44, 0x416710

void FUN_00402a44(void)

{
  int local_8c;
  undefined4 local_88 [9];
  int local_64;
  char *local_1c;
  int local_18;
  int local_14;
  int local_10;
  int local_c;
  int local_8;
  
  local_c = 0;
  local_14 = 0;
  if (DAT_0045c080 == 0) {
    _memset(local_88,0,0x6c);
    local_88[0] = 0x6c;
    DAT_0045c080 = 1;
    local_1c = s__columbus_art_endgame_a00_tga_0045c0fc;
    FUN_00416710(s__columbus_art_endgame_a00_tga_0045c0fc,&DAT_0045c078,1,(uint *)&DAT_00468c9c,
                 (uint *)&DAT_00468b94);
    FUN_00416710(local_1c,&DAT_0045c07c,0,(uint *)0x0,(uint *)0x0);
    (**(code **)(*DAT_0045c07c + 100))(DAT_0045c07c,0,local_88,1,0);
    local_8c = local_64;
    local_8 = (DAT_00468ca0 * local_14 + local_c) * 2 + DAT_0045c040;
    for (local_10 = 0; local_10 < DAT_00468b94; local_10 = local_10 + 1) {
      for (local_18 = 0; local_18 < DAT_00468c9c; local_18 = local_18 + 1) {
        *(undefined2 *)(local_8c + local_18 * 2) = *(undefined2 *)(local_8 + local_18 * 2);
      }
      local_8 = local_8 + DAT_00468ca0 * 2;
      local_8c = local_8c + DAT_00468c9c * 2;
    }
    (**(code **)(*DAT_0045c07c + 0x80))(DAT_0045c07c,0);
  }
  return;
}
