// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x429d47
// name: FUN_00429d47
// ranges: 0x429d47-0x42a004
// calls: 0x429d1a, 0x429d47

undefined4 __cdecl FUN_00429d47(int param_1,int param_2,int param_3)

{
  undefined4 uVar1;
  
  if (param_2 < param_1) {
    FUN_00429d1a(&param_1,&param_2);
  }
  if (param_3 < param_1) {
    FUN_00429d1a(&param_1,&param_3);
  }
  if (param_3 < param_2) {
    FUN_00429d1a(&param_2,&param_3);
  }
  if (((param_1 == 0) && (param_2 == 1)) && (param_3 == 2)) {
    uVar1 = 0;
  }
  else if (((param_1 == 2) && (param_2 == 0xe)) && (param_3 == 0x17)) {
    uVar1 = 1;
  }
  else if (((param_1 == 0x15) && (param_2 == 0x16)) && (param_3 == 0x17)) {
    uVar1 = 2;
  }
  else if (((param_1 == 0) && (param_2 == 9)) && (param_3 == 0x15)) {
    uVar1 = 3;
  }
  else if (((param_1 == 3) && (param_2 == 4)) && (param_3 == 5)) {
    uVar1 = 4;
  }
  else if (((param_1 == 5) && (param_2 == 0xd)) && (param_3 == 0x14)) {
    uVar1 = 5;
  }
  else if (((param_1 == 0x12) && (param_2 == 0x13)) && (param_3 == 0x14)) {
    uVar1 = 6;
  }
  else if (((param_1 == 3) && (param_2 == 10)) && (param_3 == 0x12)) {
    uVar1 = 7;
  }
  else if (((param_1 == 6) && (param_2 == 7)) && (param_3 == 8)) {
    uVar1 = 8;
  }
  else if (((param_1 == 8) && (param_2 == 0xc)) && (param_3 == 0x11)) {
    uVar1 = 9;
  }
  else if (((param_1 == 0xf) && (param_2 == 0x10)) && (param_3 == 0x11)) {
    uVar1 = 10;
  }
  else if (((param_1 == 6) && (param_2 == 0xb)) && (param_3 == 0xf)) {
    uVar1 = 0xb;
  }
  else if (((param_1 == 1) && (param_2 == 4)) && (param_3 == 7)) {
    uVar1 = 0xc;
  }
  else if (((param_1 == 0xc) && (param_2 == 0xd)) && (param_3 == 0xe)) {
    uVar1 = 0xd;
  }
  else if (((param_1 == 0x10) && (param_2 == 0x13)) && (param_3 == 0x16)) {
    uVar1 = 0xe;
  }
  else {
    uVar1 = 0xf;
  }
  return uVar1;
}
