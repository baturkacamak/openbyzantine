// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x40271b
// name: FUN_0040271b
// ranges: 0x40271b-0x40278b
// calls: 0x40271b

undefined4 __cdecl FUN_0040271b(short param_1,short param_2)

{
  short sVar1;
  undefined4 uVar2;
  
  if ((param_1 < 0x139) || (0x886 < param_1)) {
    uVar2 = 1;
  }
  else {
    sVar1 = param_2 + -1 + param_1;
    if ((sVar1 < 0x139) || (0x886 < sVar1)) {
      uVar2 = 1;
    }
    else {
      uVar2 = 0;
    }
  }
  return uVar2;
}
