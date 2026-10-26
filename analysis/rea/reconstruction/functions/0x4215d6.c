// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4215d6
// name: FUN_004215d6
// ranges: 0x4215d6-0x42168f
// calls: 0x421373, 0x42151e, 0x4215d6, 0x421b8c, 0x44bc20

int __cdecl FUN_004215d6(int param_1)

{
  int iVar1;
  int local_14;
  undefined4 *local_10;
  FILE *local_c;
  int local_8;
  
  local_c = (FILE *)FUN_0044bc20(PTR_s_savegame_notebook_000_0045f4e0,&DAT_0045fce0);
  if (local_c == (FILE *)0x0) {
    local_14 = 0;
  }
  else {
    FUN_0042151e();
    local_8 = 0;
    local_14 = 0;
    do {
      do {
        if ((local_c->_flag & 0x10) != 0) goto LAB_00421677;
        local_10 = FUN_00421373(local_c,param_1,local_8);
        iVar1 = local_8;
      } while (local_10 == (undefined4 *)0x0);
      local_8 = local_8 + 1;
      FUN_00421b8c(local_10,&local_14,iVar1);
    } while (local_14 != 100);
LAB_00421677:
    _fclose(local_c);
  }
  return local_14;
}
