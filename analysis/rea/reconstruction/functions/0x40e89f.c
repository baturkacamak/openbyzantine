// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x40e89f
// name: FUN_0040e89f
// ranges: 0x40e89f-0x40ea8a
// calls: 0x40e89f, 0x44ba00, 0x44cc60

int __cdecl FUN_0040e89f(char *param_1,int param_2,int param_3)

{
  int iVar1;
  int local_234;
  DWORD local_230 [5];
  uint local_21c [65];
  int local_118;
  HANDLE local_114;
  uint local_110 [65];
  char *local_c;
  int local_8;
  
  local_8 = 0;
  local_c = _strrchr(param_1,0x5c);
  local_114 = __findfirst(param_1,local_230);
  if (local_114 == (HANDLE)0xffffffff) {
    local_8 = 0;
  }
  else {
    if (local_c != (char *)0x0) {
      *local_c = '\0';
    }
    do {
      if (local_c == (char *)0x0) {
        FUN_0044ba00(*(uint **)(param_2 + local_8 * 4),local_21c);
      }
      else {
        _sprintf(*(char **)(param_2 + local_8 * 4),s__s__s_0045e75c,param_1,local_21c);
      }
      local_8 = local_8 + 1;
    } while ((local_8 != param_3) && (iVar1 = __findnext(local_114,local_230), iVar1 != -1));
    FUN_0044cc60(local_114);
    for (local_118 = 0; local_118 < local_8; local_118 = local_118 + 1) {
      for (local_234 = 0; local_234 < local_8 - (local_118 + 1); local_234 = local_234 + 1) {
        iVar1 = __strcmpi(*(char **)(param_2 + local_234 * 4),
                          *(char **)(param_2 + 4 + local_234 * 4));
        if (0 < iVar1) {
          FUN_0044ba00(local_110,*(uint **)(param_2 + local_234 * 4));
          FUN_0044ba00(*(uint **)(param_2 + local_234 * 4),*(uint **)(param_2 + 4 + local_234 * 4));
          FUN_0044ba00(*(uint **)(param_2 + 4 + local_234 * 4),local_110);
        }
      }
    }
    if (local_c != (char *)0x0) {
      *local_c = '\\';
    }
  }
  return local_8;
}
