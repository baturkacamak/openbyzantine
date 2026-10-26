// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x421690
// name: FUN_00421690
// ranges: 0x421690-0x4216fd
// calls: 0x421690

undefined4 __cdecl FUN_00421690(int param_1)

{
  int iVar1;
  int local_8;
  
  local_8 = 0;
  while( true ) {
    if (0xe < local_8) {
      return DAT_0046c89c;
    }
    iVar1 = _strncmp(&DAT_00483bc0 + param_1 * 9,(&PTR_DAT_0045f4e8)[local_8],2);
    if (iVar1 == 0) break;
    local_8 = local_8 + 1;
  }
  return *(undefined4 *)(&DAT_0046c888 + local_8 * 4);
}
