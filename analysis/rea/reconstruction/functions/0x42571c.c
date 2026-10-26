// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x42571c
// name: FUN_0042571c
// ranges: 0x42571c-0x425951
// calls: 0x40ac38, 0x40ac43, 0x42571c

bool __cdecl FUN_0042571c(int param_1,int param_2,FILE *param_3,char param_4,byte param_5)

{
  bool bVar1;
  undefined4 local_14;
  undefined1 local_10;
  undefined1 local_c;
  
  bVar1 = false;
  if (param_2 == 0) {
    _fputc(0xff,param_3);
    _fputc((uint)param_5,param_3);
  }
  else if (param_4 == '\0') {
    local_c = 0xff;
    do {
      local_c = local_c + 1;
      local_14 = *(int *)(param_1 + 0x4c4 + (uint)local_c * 4);
      if ((local_14 != 0) && (local_14 != param_2)) {
        local_10 = 0;
        for (; ((local_10 < param_5 && (local_14 != param_2)) && (local_14 != 0));
            local_14 = *(int *)(local_14 + 8)) {
          local_10 = local_10 + 1;
        }
      }
    } while ((local_c < 0x31) && (local_14 != param_2));
    if (local_c != 0x31) {
      _fputc((uint)local_c,param_3);
      _fputc((uint)param_5,param_3);
    }
    else {
      FUN_0040ac43();
      FUN_0040ac38();
      _fputc(0xff,param_3);
      _fputc((uint)param_5,param_3);
    }
    bVar1 = local_c == 0x31;
  }
  else {
    local_c = 0;
    while ((local_c < 0x32 && (*(int *)(param_1 + 0x58c + (uint)local_c * 4) != param_2))) {
      local_c = local_c + 1;
    }
    if (local_c == 0x32) {
      FUN_0040ac43();
      FUN_0040ac38();
      _fputc(0xff,param_3);
      _fputc((uint)param_5,param_3);
      bVar1 = true;
    }
    else {
      _fputc((uint)local_c,param_3);
      _fputc((uint)param_5,param_3);
    }
  }
  return bVar1;
}
