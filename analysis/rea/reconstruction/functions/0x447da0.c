// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x447da0
// name: FUN_00447da0
// ranges: 0x447da0-0x447dc5
// calls: 0x447da0

int __cdecl FUN_00447da0(int param_1,int param_2)

{
  int iVar1;
  int iVar2;
  
  iVar1 = 0;
  iVar2 = 0;
  if (0 < param_2) {
    do {
      if (iVar2 < *(int *)(param_1 + 0x18)) {
        iVar1 = param_1;
        iVar2 = *(int *)(param_1 + 0x18);
      }
      param_1 = param_1 + 0x20;
      param_2 = param_2 + -1;
    } while (param_2 != 0);
  }
  return iVar1;
}
