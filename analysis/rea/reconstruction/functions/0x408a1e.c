// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x408a1e
// name: FUN_00408a1e
// ranges: 0x408a1e-0x408adb
// calls: 0x40892d, 0x408a1e, 0x44a460

void __cdecl FUN_00408a1e(FILE *param_1)

{
  undefined4 local_8;
  
  FUN_0044a460((char *)&DAT_0045d1c0,4,1,param_1);
  FUN_0044a460((char *)&DAT_0045d1c4,4,1,param_1);
  FUN_0044a460((char *)&DAT_0045d1bc,4,1,param_1);
  FUN_0044a460(&DAT_00468cc0,0x28,1,param_1);
  FUN_0044a460(&DAT_00468d78,14000,1,param_1);
  if (DAT_0045d1c8 == 0) {
    local_8 = 0xffffffff;
  }
  else {
    local_8 = *(undefined4 *)(DAT_0045d1c8 + 8);
  }
  FUN_0044a460((char *)&local_8,4,1,param_1);
  FUN_0040892d(param_1);
  return;
}
