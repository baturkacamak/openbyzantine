// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x409981
// name: FUN_00409981
// ranges: 0x409981-0x409a11
// calls: 0x409457, 0x4094f2, 0x409981, 0x40a40d, 0x40a4bc

void __cdecl FUN_00409981(int param_1)

{
  if (DAT_00485c6c != '\0') {
    FUN_00409457(s_RANDOM_0045d420,'\x01');
    FUN_0040a4bc(param_1,0,'\0','\x01');
    if (*(char *)(param_1 + 2) == '\x02') {
      FUN_0040a4bc(param_1,1,'\x01','\x01');
    }
    else if (*(char *)(param_1 + 2) == '@') {
      FUN_0040a40d(param_1,1,'\x01');
    }
    FUN_004094f2();
  }
  return;
}
