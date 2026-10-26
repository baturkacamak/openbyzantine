// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x41d3d5
// name: FUN_0041d3d5
// ranges: 0x41d3d5-0x41d430
// calls: 0x41d3d5

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int FUN_0041d3d5(void)

{
  int iVar1;
  char local_8;
  
  local_8 = '\0';
  while (local_8 == '\0') {
    _DAT_00462f04 = _DAT_00462f04 + -1;
    if (_DAT_00462f04 < 0) {
      iVar1 = __filbuf((FILE *)&PTR_DAT_00462f00);
      local_8 = (char)iVar1;
    }
    else {
      local_8 = *PTR_DAT_00462f00;
      PTR_DAT_00462f00 = PTR_DAT_00462f00 + 1;
    }
  }
  return (int)local_8;
}
