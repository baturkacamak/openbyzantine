// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x415a4e
// name: FUN_00415a4e
// ranges: 0x415a4e-0x415bcb
// calls: 0x401256, 0x406cfe, 0x407160, 0x408e3f, 0x40c5f0, 0x40dd7f, 0x413ac0, 0x414be7, 0x415a4e, 0x415bcc, 0x415c42, 0x4163ab, 0x4163f3, 0x42a9d0, 0x43db30, 0x44d060

undefined4 __cdecl FUN_00415a4e(undefined4 param_1,undefined4 param_2,undefined4 param_3)

{
  bool bVar1;
  int iVar2;
  undefined4 uVar3;
  undefined3 extraout_var;
  undefined3 extraout_var_00;
  
  iVar2 = FUN_00415bcc(param_1,param_2,param_3);
  if (iVar2 == 0) {
    uVar3 = 0;
  }
  else {
    iVar2 = FUN_00415c42();
    if (iVar2 == 0) {
      FUN_0040c5f0();
      iVar2 = FUN_0040dd7f();
      if (iVar2 == 0) {
        iVar2 = FUN_0043db30();
        if (iVar2 == 0) {
          uVar3 = 0;
        }
        else {
          if (DAT_0048688c == 0) {
            bVar1 = FUN_00413ac0(DAT_0046c4e8,DAT_00486a04,DAT_004869d4,DAT_00486888,DAT_00461934,0)
            ;
            if (CONCAT31(extraout_var_00,bVar1) != 0) {
              return 0;
            }
          }
          else {
            bVar1 = FUN_00413ac0(DAT_0046c4e8,DAT_00486a04,DAT_004869d4,DAT_0048688c,DAT_00461934,1)
            ;
            if (CONCAT31(extraout_var,bVar1) != 0) {
              return 0;
            }
          }
          FUN_004163f3();
          iVar2 = FUN_0042a9d0();
          if (iVar2 == 0) {
            uVar3 = 0;
          }
          else {
            iVar2 = FUN_00407160();
            if (iVar2 == 0) {
              iVar2 = FUN_00408e3f();
              if (iVar2 == 0) {
                FUN_004163ab();
                iVar2 = FUN_00401256();
                if (iVar2 == 0) {
                  FUN_00406cfe();
                  FUN_0044d060(0xbeef);
                  uVar3 = 1;
                }
                else {
                  uVar3 = 0;
                }
              }
              else {
                uVar3 = 0;
              }
            }
            else {
              uVar3 = 0;
            }
          }
        }
      }
      else {
        uVar3 = 0;
      }
    }
    else {
      FUN_00414be7(iVar2);
      uVar3 = 0;
    }
  }
  return uVar3;
}
