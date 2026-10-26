// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4281b1
// name: FUN_004281b1
// ranges: 0x4281b1-0x428228
// calls: 0x422927, 0x428143, 0x4281b1

undefined4 __cdecl FUN_004281b1(int param_1)

{
  int *piVar1;
  int iVar2;
  undefined4 uVar3;
  
  iVar2 = FUN_00428143(param_1);
  if (iVar2 == 0) {
    piVar1 = *(int **)(*(int *)(&DAT_00485c90 + param_1 * 4) + 8);
    if (piVar1 == (int *)0x0) {
      uVar3 = 0;
    }
    else {
      if (piVar1 == DAT_0045fd14) {
        FUN_00422927();
      }
      else {
        (**(code **)(*piVar1 + 0x48))(piVar1);
      }
      uVar3 = 1;
    }
  }
  else {
    uVar3 = 0;
  }
  return uVar3;
}
