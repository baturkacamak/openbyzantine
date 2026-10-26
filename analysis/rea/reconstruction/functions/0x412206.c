// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x412206
// name: FUN_00412206
// ranges: 0x412206-0x4123f8
// calls: 0x411b4b, 0x411db1, 0x412206, 0x44ba00, 0x44bc20, 0x44c2f0, 0x44c580, 0x44ce20

int __cdecl FUN_00412206(uint *param_1,int param_2)

{
  int iVar1;
  uint uVar2;
  char *pcVar3;
  undefined4 uVar4;
  int local_11c;
  char *local_118;
  char *local_114;
  FILE *local_110;
  int local_10c;
  char local_108 [260];
  
  local_11c = -1;
  if (DAT_0045e7a8 == 0) {
    _sprintf(local_108,s_engine__s_0045ea7c,param_1);
    local_110 = (FILE *)FUN_0044bc20(local_108,&DAT_0045ea88);
  }
  else {
    local_110 = FUN_00411b4b(param_1,&local_11c);
  }
  if (local_110 == (FILE *)0x0) {
    local_10c = 0;
  }
  else {
    local_10c = 0;
    while (uVar2 = FUN_00411db1(&local_110->_ptr,local_11c), uVar2 == 0) {
      local_114 = (char *)0x0;
      do {
        local_118 = local_114 + 0x47fa70;
        local_118 = FUN_0044ce20(local_118,0x800 - (int)local_114,local_110);
        pcVar3 = FUN_0044c580((uint *)&DAT_0047fa70);
        local_114 = pcVar3;
        if (pcVar3[0x47fa6e] == '\r') {
          pcVar3[0x47fa6e] = '\n';
          local_114 = pcVar3 + -1;
          pcVar3[0x47fa6f] = '\0';
        }
      } while ((local_118 != (char *)0x0) && (*local_118 != '$'));
      if (local_118 == (char *)0x0) break;
      *local_118 = '\0';
      uVar4 = FUN_0044c2f0((SIZE_T)local_114);
      *(undefined4 *)(param_2 + local_10c * 4) = uVar4;
      iVar1 = local_10c * 4;
      local_10c = local_10c + 1;
      FUN_0044ba00(*(uint **)(param_2 + iVar1),(uint *)&DAT_0047fa70);
    }
    _fclose(local_110);
  }
  return local_10c;
}
