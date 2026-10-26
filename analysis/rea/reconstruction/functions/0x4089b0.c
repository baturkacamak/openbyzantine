// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4089b0
// name: FUN_004089b0
// ranges: 0x4089b0-0x408a1d
// calls: 0x4089b0

void __cdecl FUN_004089b0(FILE *param_1)

{
  int iVar1;
  int local_10;
  
  local_10 = 0;
  while ((local_10 < 0x15e && (iVar1 = _fgetc(param_1), (char)iVar1 != -1))) {
    __fread_lk(*(char **)(&DAT_00486f90 + local_10 * 4),1,1,param_1);
    local_10 = local_10 + 1;
  }
  return;
}
