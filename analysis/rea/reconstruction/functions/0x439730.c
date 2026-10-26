// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x439730
// name: FUN_00439730
// ranges: 0x439730-0x4397ab
// calls: 0x439730

undefined2 __cdecl FUN_00439730(char *param_1)

{
  int iVar1;
  int local_8;
  
  local_8 = 0;
  while( true ) {
    if (2999 < local_8) {
      return 0;
    }
    if ((*(int *)(&DAT_00477350 + local_8 * 4) != 0) &&
       (iVar1 = __strcmpi(*(char **)(&DAT_00477350 + local_8 * 4),param_1), iVar1 == 0)) break;
    local_8 = local_8 + 1;
  }
  return *(undefined2 *)(&DAT_0047e2f0 + local_8 * 2);
}
