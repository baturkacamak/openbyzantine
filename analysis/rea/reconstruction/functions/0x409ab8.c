// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x409ab8
// name: FUN_00409ab8
// ranges: 0x409ab8-0x409b5d
// calls: 0x409457, 0x4094f2, 0x409ab8, 0x40a40d, 0x40a4bc

void __cdecl FUN_00409ab8(int param_1)

{
  if (DAT_00485c6c != '\0') {
    FUN_00409457(&DAT_0045d434,'\x01');
    if (*(char *)(param_1 + 1) == '\x02') {
      FUN_0040a4bc(param_1,0,'\0','\x01');
    }
    else {
      FUN_0040a40d(param_1,0,'\0');
    }
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
