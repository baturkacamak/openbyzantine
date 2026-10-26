// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x422375
// name: FUN_00422375
// ranges: 0x422375-0x4223c0
// calls: 0x422375

undefined4 __cdecl FUN_00422375(char *param_1,char *param_2)

{
  undefined4 uVar1;
  
  if (*param_1 == '$') {
    param_1 = param_1 + 1;
  }
  if ((*param_1 == '<') && (*param_2 == '>')) {
    uVar1 = 0;
  }
  else {
    uVar1 = 1;
  }
  return uVar1;
}
