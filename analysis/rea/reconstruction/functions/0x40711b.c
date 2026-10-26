// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x40711b
// name: FUN_0040711b
// ranges: 0x40711b-0x40715f
// calls: 0x40711b, 0x44c520

void __cdecl FUN_0040711b(char *param_1)

{
  undefined4 local_8;
  
  local_8 = FUN_0044c520(param_1,'\n');
  while (local_8 != (char *)0x0) {
    *local_8 = ' ';
    local_8 = FUN_0044c520(param_1,'\n');
  }
  return;
}
