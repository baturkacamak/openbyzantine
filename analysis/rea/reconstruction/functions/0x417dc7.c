// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x417dc7
// name: FUN_00417dc7
// ranges: 0x417dc7-0x417e9b
// calls: 0x417dc7

undefined4 __cdecl FUN_00417dc7(int param_1,int param_2)

{
  undefined4 uVar1;
  
  if ((((DAT_0045f218 == 0) || (DAT_0045d1c8 != 0)) || (param_1 < 0x118)) ||
     (((0x172 < param_1 || (param_2 < 0x14)) || (0x1a3 < param_2)))) {
    DAT_0045f208 = 0;
    DAT_0045f20c = 0;
    uVar1 = 0;
  }
  else if (param_2 < 0x28) {
    DAT_0045f208 = 1;
    DAT_0045f20c = 0;
    uVar1 = 1;
  }
  else {
    DAT_0045f208 = 0;
    if (param_2 < 0x191) {
      DAT_0045f20c = 0;
      uVar1 = 0;
    }
    else {
      DAT_0045f20c = 1;
      uVar1 = 1;
    }
  }
  return uVar1;
}
