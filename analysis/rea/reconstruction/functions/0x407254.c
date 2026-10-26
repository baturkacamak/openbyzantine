// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x407254
// name: FUN_00407254
// ranges: 0x407254-0x4073c2
// calls: 0x407254, 0x40c624, 0x416710, 0x44c630

undefined4 __cdecl FUN_00407254(int param_1)

{
  undefined4 uVar1;
  int iVar2;
  uint local_124;
  undefined1 local_120 [8];
  undefined1 local_118;
  uint local_114;
  char *local_110;
  int local_10c;
  uint local_108 [65];
  
  if (param_1 == 0) {
    uVar1 = 1;
  }
  else if (*(int *)(param_1 + 0xc) == 0) {
    local_10c = *(int *)(param_1 + 8);
    if ((&DAT_00482540)[local_10c] == 0) {
      local_110 = (char *)**(undefined4 **)(&DAT_00485400 + local_10c * 4);
    }
    else {
      local_110 = (char *)(&DAT_00482540)[local_10c];
    }
    FUN_0044c630(local_120,local_110,8);
    local_118 = 0;
    FUN_0040c624(local_108,(uint *)s_inventor_0045d200,(uint *)local_120,(uint *)&DAT_0045d1f8);
    iVar2 = FUN_00416710((char *)local_108,(undefined4 *)(param_1 + 0xc),1,&local_124,&local_114);
    if (iVar2 == 0) {
      local_120[0] = 'h';
      FUN_0040c624(local_108,(uint *)s_inventor_hotitems_0045d214,(uint *)local_120,
                   (uint *)&DAT_0045d20c);
      iVar2 = FUN_00416710((char *)local_108,(undefined4 *)(param_1 + 0x14),1,&local_124,&local_114)
      ;
      if (iVar2 == 0) {
        *(undefined4 *)(param_1 + 0x10) = *(undefined4 *)(param_1 + 0xc);
        uVar1 = 0;
      }
      else {
        uVar1 = 1;
      }
    }
    else {
      uVar1 = 1;
    }
  }
  else {
    uVar1 = 0;
  }
  return uVar1;
}
