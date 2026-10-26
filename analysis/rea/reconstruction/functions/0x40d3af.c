// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x40d3af
// name: FUN_0040d3af
// ranges: 0x40d3af-0x40d47d; 0x40d483-0x40d496; 0x40d4b3-0x40d54c
// calls: 0x40d3af, 0x40d9f7, 0x413bdd, 0x416220, 0x41624a, 0x4196a4, 0x419d28, 0x41d440, 0x42b280

undefined4 __cdecl
FUN_0040d3af(undefined4 param_1,int param_2,int param_3,int param_4,int param_5,int param_6)

{
  int iVar1;
  bool bVar2;
  int local_8;
  
  FUN_0040d9f7();
  switch(param_2) {
  case 0:
  case 3:
    param_4 = param_4 + 0x14;
    local_8 = 1;
    break;
  case 1:
    FUN_00419d28(DAT_0047a8a4);
    FUN_0042b280(DAT_0046e82c,0x4b000);
    param_4 = param_4 + 0x14;
  case 6:
    local_8 = 2;
    break;
  case 2:
  case 4:
    iVar1 = FUN_0041d440(1);
    if ((iVar1 == 0) && (param_6 != 0)) {
      FUN_004196a4(0,DAT_00477344,0);
      iVar1 = FUN_00416220();
      if (iVar1 == 0) {
        FUN_0042b280(DAT_0046e82c,0x41a00);
      }
    }
    param_4 = param_4 + 0x14;
    local_8 = 2;
    break;
  case 5:
    param_4 = param_4 + 0x14;
  default:
    local_8 = 0;
  }
  if ((local_8 == 2) && ((iVar1 = FUN_0041d440(1), iVar1 == 0 || (param_2 == 6)))) {
    local_8 = 0;
    param_3 = param_3 + 0xa0;
    param_4 = param_4 + 100;
  }
  FUN_0041624a();
  bVar2 = DAT_0045d1d0 != 0;
  if (bVar2) {
    DAT_0045d1d0 = 0;
  }
  FUN_00413bdd(param_1,param_3,param_4,local_8,param_5);
  if (bVar2) {
    DAT_0045d1d0 = 1;
  }
  return 1;
}
