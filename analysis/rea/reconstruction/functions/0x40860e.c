// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x40860e
// name: FUN_0040860e
// ranges: 0x40860e-0x408661
// calls: 0x40860e, 0x44c630

undefined4 __cdecl FUN_0040860e(char *param_1)

{
  undefined4 uVar1;
  
  if (DAT_0045d1cc == 0) {
    uVar1 = 0xffffffff;
  }
  else {
    FUN_0044c630(param_1,(char *)(&DAT_00482540)[*(int *)(DAT_0045d1cc + 8)],8);
    param_1[8] = '\0';
    *param_1 = 'e';
    uVar1 = 0;
  }
  return uVar1;
}
