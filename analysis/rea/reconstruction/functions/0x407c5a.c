// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x407c5a
// name: FUN_00407c5a
// ranges: 0x407c5a-0x407ca6; 0x407cac-0x407d50; 0x407d56-0x407d71; 0x407dcd-0x407df8
// calls: 0x407c5a

bool __cdecl FUN_00407c5a(int param_1,char param_2)

{
  int local_8;
  
  switch(param_2) {
  case '\0':
  case '\x01':
  case '\x02':
  case '\x05':
  case '\b':
  case '\t':
  case '\f':
    if (param_1 == -1) {
      return true;
    }
    break;
  case '\x03':
  case '\x04':
  case '\x06':
  case '\a':
    return false;
  case '\n':
  case '\v':
  case '\r':
  case '\x0e':
  case '\x10':
    if (param_1 == -1) {
      if (DAT_0045d1c8 != 0) {
        return false;
      }
      return true;
    }
    break;
  case '\x0f':
  case '\x11':
  case '\x1d':
  case '\x1e':
    return false;
  case '\x12':
  case '\x13':
  case '\x14':
    if (param_1 == -1) {
      return false;
    }
    if (param_2 != '\x12') {
      local_8 = 0;
      while( true ) {
        if (*(int *)(*(int *)(&DAT_00486a10 + param_1 * 4) + 100) <= local_8) {
          return false;
        }
        if (*(int *)(&DAT_00486f90 +
                    *(int *)(*(int *)(&DAT_00486a10 + param_1 * 4) + local_8 * 4) * 4) ==
            DAT_0045d1c8) break;
        local_8 = local_8 + 1;
      }
      return true;
    }
    break;
  case '\x18':
  case '\x19':
  case '\x1a':
  case '\x1b':
    return false;
  }
  return *(int *)(&DAT_00486f90 + param_1 * 4) == DAT_0045d1c8;
}
