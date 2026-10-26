// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x41df43
// name: FUN_0041df43
// ranges: 0x41df43-0x41e001
// calls: 0x401184, 0x41045b, 0x41df43, 0x42b280, 0x43b216

void FUN_0041df43(void)

{
  undefined1 uVar1;
  
  uVar1 = DAT_004859cd;
  if (DAT_0045c060 == 0) {
    if ((DAT_0048253e == 0xffff) || (*(int *)(&DAT_00484e80 + (uint)DAT_0048253e * 4) == 0)) {
      FUN_0042b280(DAT_0046e82c,0x4b000);
    }
    else {
      FUN_0041045b((int)DAT_0046e82c,(uint)DAT_0048253e,100);
      DAT_004859cd = 1;
      FUN_0043b216((undefined4 *)0x0,1,1);
      DAT_004859cd = uVar1;
    }
  }
  else {
    DAT_0045c060 = 0;
    FUN_00401184(1);
    DAT_0045c060 = 1;
  }
  return;
}
