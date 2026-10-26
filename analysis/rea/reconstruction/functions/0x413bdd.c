// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x413bdd
// name: FUN_00413bdd
// ranges: 0x413bdd-0x413d00
// calls: 0x413bdd, 0x414c3d, 0x41624a, 0x424981, 0x439a28, 0x439cce

void __cdecl
FUN_00413bdd(undefined4 param_1,undefined4 param_2,undefined4 param_3,int param_4,int param_5)

{
  int iVar1;
  int *local_8;
  
  local_8 = DAT_00486888;
  if (DAT_0046c4dc != (void *)0x0) {
    if (DAT_0048688c != (int *)0x0) {
      (**(code **)(*DAT_0048688c + 0x1c))(DAT_0048688c,0,0,DAT_004869d4,0,0x10);
      local_8 = DAT_0048688c;
    }
    while (DAT_00460198 != 0) {
      FUN_0041624a();
    }
    iVar1 = FUN_00439a28(DAT_0046c4dc,param_1,param_4,param_2,param_3,param_5);
    if (iVar1 == 0) {
      DAT_0045f060 = 0;
      DAT_0045f050 = 0;
      while (DAT_0045f060 == 0) {
        FUN_0041624a();
        if ((DAT_0045fdac != 0) && (DAT_0045f054 != 0)) {
          FUN_00424981(local_8);
        }
        if (DAT_0045f050 != 0) {
          FUN_00439cce((int)DAT_0046c4dc);
          DAT_0045f050 = 0;
        }
      }
      FUN_00414c3d((byte)DAT_0045f064);
    }
  }
  return;
}
