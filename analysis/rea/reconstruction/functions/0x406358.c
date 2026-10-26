// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x406358
// name: FUN_00406358
// ranges: 0x406358-0x4063df
// calls: 0x406358

undefined4 __cdecl FUN_00406358(char param_1,int param_2)

{
  undefined4 uVar1;
  
  if (DAT_0045d0ac == 0) {
    uVar1 = 0;
  }
  else if ((param_1 == 'f') &&
          ((((param_2 == 0 || (param_2 == 4)) || (param_2 == 5)) || (param_2 == 6)))) {
    uVar1 = 1;
  }
  else if ((param_1 == '\'') && (param_2 == 0)) {
    uVar1 = 0xfffffffe;
  }
  else {
    uVar1 = 0;
  }
  return uVar1;
}
