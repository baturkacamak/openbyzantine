// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x409bde
// name: FUN_00409bde
// ranges: 0x409bde-0x409c5d
// calls: 0x409457, 0x4094f2, 0x409bde, 0x40a40d, 0x40a4bc

void __cdecl FUN_00409bde(int param_1)

{
  if (DAT_00485c6c != '\0') {
    FUN_00409457(s_DIVIDE_0045d44c,'\x01');
    FUN_0040a4bc(param_1,0,'\0','\x01');
    if (*(char *)(param_1 + 2) == '\x02') {
      FUN_0040a4bc(param_1,1,'\x01','\x01');
    }
    else {
      FUN_0040a40d(param_1,1,'\x01');
    }
    FUN_004094f2();
  }
  return;
}
