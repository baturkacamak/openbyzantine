// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x455c90
// name: FUN_00455c90
// ranges: 0x455c90-0x455cfd
// calls: 0x455c90

void __cdecl FUN_00455c90(byte *param_1,uint param_2)

{
  byte bVar1;
  byte *pbVar2;
  byte *pbVar3;
  
  pbVar2 = (byte *)0x0;
  if (DAT_00463a1c == 0) {
    _strrchr((char *)param_1,param_2);
    return;
  }
  do {
    bVar1 = *param_1;
    if ((*(byte *)((int)&DAT_00463918 + bVar1 + 1) & 4) == 0) {
      pbVar3 = param_1;
      if (bVar1 == param_2) {
LAB_00455cf0:
        pbVar2 = param_1;
        pbVar3 = pbVar2;
      }
    }
    else {
      pbVar3 = param_1 + 1;
      if (param_1[1] == 0) {
        param_1 = pbVar3;
        if (pbVar2 == (byte *)0x0) goto LAB_00455cf0;
      }
      else if (CONCAT11(bVar1,param_1[1]) == param_2) {
        pbVar2 = param_1;
      }
    }
    param_1 = pbVar3 + 1;
    if (*pbVar3 == 0) {
      return;
    }
  } while( true );
}
