// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x43db30
// name: FUN_0043db30
// ranges: 0x43db30-0x43db98; 0x43db9e-0x43dbf9
// calls: 0x4161f6, 0x43db30, 0x43dc46

undefined4 FUN_0043db30(void)

{
  undefined4 uVar1;
  int iVar2;
  
  uVar1 = FUN_004161f6();
  DAT_00461934 = (int *)0x0;
  iVar2 = DirectSoundCreate(0,&DAT_00461934,0);
  if ((iVar2 == 0) || (iVar2 == -0x7787fff6)) {
    iVar2 = (**(code **)(*DAT_00461934 + 0x18))(DAT_00461934,uVar1,1);
    if (iVar2 == 0) {
      iVar2 = FUN_0043dc46((undefined4 *)&DAT_0046e23c);
      if (iVar2 == 0) {
        uVar1 = 0;
      }
      else {
        uVar1 = 1;
      }
    }
    else {
      uVar1 = 0;
    }
  }
  else if (iVar2 == -0x7787ff7e) {
    uVar1 = 1;
  }
  else {
    uVar1 = 0;
  }
  return uVar1;
}
