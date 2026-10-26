// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x40ced0
// name: FUN_0040ced0
// ranges: 0x40ced0-0x40cf72
// calls: 0x40ced0

int __cdecl FUN_0040ced0(char *param_1)

{
  int iVar1;
  int local_8;
  
  local_8 = 0;
  while( true ) {
    if (0xf < local_8) {
      iVar1 = __strcmpi(&DAT_0045dadc,param_1);
      if (iVar1 == 0) {
        iVar1 = -2;
      }
      else {
        iVar1 = __strcmpi(&DAT_0045dae0,param_1);
        if (iVar1 == 0) {
          iVar1 = -1;
        }
        else {
          iVar1 = -3;
        }
      }
      return iVar1;
    }
    iVar1 = __strcmpi((&PTR_DAT_0045f4a0)[local_8],param_1);
    if (iVar1 == 0) break;
    local_8 = local_8 + 1;
  }
  return local_8;
}
