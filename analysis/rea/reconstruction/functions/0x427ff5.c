// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x427ff5
// name: FUN_00427ff5
// ranges: 0x427ff5-0x428070
// calls: 0x427ff5

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __cdecl FUN_00427ff5(undefined4 param_1,int param_2)

{
  int *piVar1;
  
  piVar1 = DAT_00486218;
  if (DAT_00486218 != (int *)0x0) {
    (**(code **)(*DAT_00486218 + 0x34))(DAT_00486218,0);
    if (param_2 == 0) {
      (**(code **)(*piVar1 + 0x30))(piVar1,0,0,1);
    }
    else {
      (**(code **)(*piVar1 + 0x30))(piVar1,0,0,0);
    }
  }
  _DAT_00460190 = 1;
  _DAT_00460194 = param_1;
  return;
}
