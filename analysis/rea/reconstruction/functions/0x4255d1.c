// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4255d1
// name: FUN_004255d1
// ranges: 0x4255d1-0x42571b
// calls: 0x40ac38, 0x40ac43, 0x4255d1

int __cdecl FUN_004255d1(int param_1,byte *param_2,FILE *param_3)

{
  char local_14;
  char local_10;
  int local_c;
  int local_8;
  
  local_8 = 0;
  if (param_2 == (byte *)0x0) {
    _fputc(0xff,param_3);
  }
  else {
    _fputc((uint)*param_2,param_3);
    local_8 = 1;
    local_10 = '\0';
    while ((local_8 == 1 && (local_10 < '2'))) {
      local_14 = '\0';
      for (local_c = *(int *)(param_1 + 0x58c + local_10 * 4);
          (local_c != 0 && (*(int *)(param_2 + 4) != local_c)); local_c = *(int *)(local_c + 8)) {
        local_14 = local_14 + '\x01';
      }
      if (*(int *)(param_2 + 4) == local_c) {
        _fputc((int)local_10,param_3);
        _fputc((int)local_14,param_3);
        local_8 = 0;
      }
      else {
        local_10 = local_10 + '\x01';
      }
    }
    if (local_8 != 0) {
      FUN_0040ac43();
      FUN_0040ac38();
      _fputc(0,param_3);
      _fputc(0,param_3);
    }
  }
  return local_8;
}
