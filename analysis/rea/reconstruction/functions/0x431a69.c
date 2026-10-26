// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x431a69
// name: FUN_00431a69
// ranges: 0x431a69-0x431b0e
// calls: 0x40c6d6, 0x40c7b4, 0x431a69

undefined4 __cdecl FUN_00431a69(int param_1)

{
  bool bVar1;
  undefined3 extraout_var;
  int iVar2;
  undefined4 uVar3;
  
  bVar1 = false;
  if (param_1 == 7) {
    bVar1 = FUN_0040c6d6(1);
    if (CONCAT31(extraout_var,bVar1) != 0) {
      DAT_0045da78 = 1;
      return 0xffff;
    }
    param_1 = 2;
    bVar1 = true;
  }
  iVar2 = FUN_0040c7b4(param_1);
  if (iVar2 == 0) {
    uVar3 = 0xffff;
  }
  else {
    if (bVar1) {
      DAT_0045f490 = 1;
    }
    else {
      DAT_0045da78 = param_1;
      DAT_0045f490 = 3;
    }
    uVar3 = 1;
  }
  return uVar3;
}
