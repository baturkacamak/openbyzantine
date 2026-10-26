// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x409901
// name: FUN_00409901
// ranges: 0x409901-0x409980
// calls: 0x409457, 0x4094f2, 0x409901, 0x40a40d, 0x40a4bc

void __cdecl FUN_00409901(int param_1)

{
  if (DAT_00485c6c != '\0') {
    FUN_00409457(&DAT_0045d418,'\x01');
    if (*(char *)(param_1 + 1) == '\x02') {
      FUN_0040a4bc(param_1,0,'\0','\x01');
    }
    else {
      FUN_0040a40d(param_1,0,'\0');
    }
    FUN_0040a4bc(param_1,1,'\x01','\0');
    FUN_004094f2();
  }
  return;
}
