// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4062bc
// name: FUN_004062bc
// ranges: 0x4062bc-0x406357
// calls: 0x4061e7, 0x4062bc, 0x44c520

int __cdecl FUN_004062bc(char *param_1,int param_2,int param_3)

{
  int iVar1;
  char *local_14;
  int local_c;
  char *local_8;
  
  local_14 = param_1;
  local_c = 0;
  while ((local_c < param_3 && (local_8 = FUN_0044c520(param_1,'\n'), local_8 != (char *)0x0))) {
    local_14 = param_1;
    param_1 = local_8 + 1;
    local_c = local_c + 1;
  }
  if (local_8 != (char *)0x0) {
    *local_8 = '\0';
  }
  iVar1 = FUN_004061e7((int)local_14,param_2);
  if (local_8 != (char *)0x0) {
    *local_8 = '\n';
  }
  return iVar1;
}
