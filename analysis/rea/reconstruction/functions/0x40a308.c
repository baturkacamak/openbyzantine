// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x40a308
// name: FUN_0040a308
// ranges: 0x40a308-0x40a387
// calls: 0x409457, 0x4094f2, 0x40a308, 0x40a388, 0x40a40d, 0x40a4bc

void __cdecl FUN_0040a308(undefined4 param_1,int param_2)

{
  FUN_00409457(param_1,'\x01');
  FUN_0040a4bc(param_2,0,'\0','\x01');
  if (*(char *)(param_2 + 2) == '\x02') {
    FUN_0040a4bc(param_2,1,'\x01','\x01');
  }
  else {
    FUN_0040a40d(param_2,1,'\x01');
  }
  FUN_0040a388(param_2,2,'\x01');
  FUN_004094f2();
  return;
}
