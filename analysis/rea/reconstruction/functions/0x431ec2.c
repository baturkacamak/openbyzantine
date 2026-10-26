// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x431ec2
// name: FUN_00431ec2
// ranges: 0x431ec2-0x4320c9; 0x4320cf-0x4320e6; 0x432103-0x432132
// calls: 0x4075fe, 0x40ac38, 0x40ac43, 0x40d9f7, 0x40da1f, 0x41045b, 0x418040, 0x41842b, 0x418bcf, 0x418eb2, 0x419a51, 0x419cf6, 0x42b29f, 0x42b2db, 0x42bce1, 0x42be07, 0x431ec2, 0x43b216
// port: Background command [partial]

void __cdecl FUN_00431ec2(ushort param_1,int param_2,int param_3)

{
  bool bVar1;
  undefined3 extraout_var;
  
  FUN_00418040((int)DAT_0046e82c);
  FUN_0042b29f((int)DAT_00477344,0x20,0x4b000);
  DAT_0048253e = param_1;
  bVar1 = FUN_0041045b((int)DAT_0046e82c,(uint)param_1,param_2);
  if (CONCAT31(extraout_var,bVar1) == 0) {
    if (((param_2 != 0x65) && (param_2 != 0x68)) &&
       (FUN_0042b2db(DAT_0047a8a4,DAT_0046e82c,0x4b000), param_2 == 4)) {
      param_2 = 100;
    }
    DAT_0045c060 = 0;
    FUN_0043b216((undefined4 *)0x0,1,param_3);
    FUN_0040d9f7();
    switch(param_2) {
    case 100:
    case 0x69:
      FUN_0041842b((int)DAT_0046e82c,0x3e800);
      break;
    case 0x65:
      FUN_00418bcf((int)DAT_0047a8a4);
      FUN_0042be07(DAT_0047a8a4,DAT_0046e82c,(int)DAT_00477344,2);
      FUN_0041045b((int)DAT_0047a8a4,(uint)DAT_0048253e,param_2);
      break;
    case 0x66:
      FUN_00418bcf((int)DAT_00477344);
      FUN_0042bce1((int)DAT_00477344,(int)DAT_0046e82c);
      break;
    case 0x67:
      FUN_00419cf6(0,(int)DAT_0046e82c,DAT_00477344);
      break;
    case 0x68:
      FUN_00418bcf((int)DAT_0047a8a4);
      FUN_00418eb2((int)DAT_0047a8a4,(int)DAT_0046e82c,DAT_00477344);
      FUN_0041045b((int)DAT_0047a8a4,(uint)DAT_0048253e,param_2);
      break;
    case 0x6a:
      FUN_00419a51((int)DAT_0046e82c,DAT_00477344);
    }
    FUN_004075fe(1);
    FUN_0042b29f((int)DAT_0046e82c,0,0x3200);
    FUN_0040da1f();
    DAT_004859cd = 0;
  }
  else {
    FUN_0040ac43();
    FUN_0040ac38();
  }
  return;
}
