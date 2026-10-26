// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4380ff
// name: FUN_004380ff
// ranges: 0x4380ff-0x438172
// calls: 0x4380ff

int __cdecl FUN_004380ff(int param_1,int param_2,FILE *param_3)

{
  int iVar1;
  int local_8;
  
  local_8 = 0;
  do {
    iVar1 = _fgetc(param_3);
    *(char *)(local_8 + param_1) = (char)iVar1;
    if ((*(char *)(local_8 + param_1) == '\0') || ((param_3->_flag & 0x10) != 0)) break;
    local_8 = local_8 + 1;
  } while (local_8 < param_2);
  if ((param_3->_flag & 0x10) != 0) {
    param_1 = 0;
  }
  return param_1;
}
