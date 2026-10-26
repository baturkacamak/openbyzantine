// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x40f135
// name: FUN_0040f135
// ranges: 0x40f135-0x40f341
// calls: 0x40f080, 0x40f135, 0x43de70, 0x43df40, 0x43dfc0, 0x43e320, 0x43e3d0, 0x43e560, 0x43e7b0, 0x43e900, 0x44bc20, 0x44c2f0

ushort * __cdecl FUN_0040f135(char *param_1,int *param_2,uint *param_3,ushort *param_4,int param_5)

{
  ushort *local_244;
  undefined4 local_240 [33];
  int local_1bc;
  int local_1b8;
  undefined4 local_178;
  undefined1 local_174;
  char local_172;
  undefined4 local_170;
  undefined1 local_16c;
  undefined4 local_168;
  int local_160;
  uint local_15c;
  int local_154;
  uint local_144;
  int local_14;
  FILE *local_10;
  undefined4 *local_c;
  ushort *local_8;
  
  local_10 = (FILE *)FUN_0044bc20(param_1,&DAT_0045e7a4);
  if (local_10 == (FILE *)0x0) {
    local_244 = (ushort *)0x0;
  }
  else {
    local_1bc = FUN_0043e900(local_240);
    FUN_0043de70(&local_1bc,0x3d,0x1a8);
    FUN_0043e7b0((int)&local_1bc,local_10);
    FUN_0043dfc0(&local_1bc,'\x01');
    local_16c = 0;
    local_170 = 1;
    if (local_172 == '\0') {
      local_168 = 0xd8;
    }
    local_178 = 1;
    local_174 = 0;
    FUN_0043e3d0(&local_1bc);
    local_14 = local_154 * local_160;
    local_c = (undefined4 *)(**(code **)(local_1b8 + 8))(&local_1bc,1,local_14,1);
    if (param_4 == (ushort *)0x0) {
      local_244 = (ushort *)FUN_0044c2f0(local_160 * local_15c * 2);
      param_5 = local_160;
      local_8 = local_244;
    }
    else {
      local_244 = param_4;
      local_8 = param_4;
      if ((local_160 != *param_2) || (local_15c != *param_3)) {
        return (ushort *)0x0;
      }
    }
    while (local_144 < local_15c) {
      FUN_0043e560(&local_1bc,local_c,1);
      FUN_0040f080((int *)*local_c,local_160,local_8);
      local_8 = local_8 + param_5;
    }
    FUN_0043e320(&local_1bc);
    FUN_0043df40((int)&local_1bc);
    _fclose(local_10);
    *param_2 = local_160;
    *param_3 = local_15c;
  }
  return local_244;
}
