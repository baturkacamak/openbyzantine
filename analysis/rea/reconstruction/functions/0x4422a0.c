// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4422a0
// name: FUN_004422a0
// ranges: 0x4422a0-0x4422b8
// calls: 0x4422a0

void __cdecl FUN_004422a0(undefined4 *param_1,undefined4 *param_2,uint param_3)

{
  int iVar1;
  
  for (iVar1 = (param_3 & 0x1ffffff) << 5; iVar1 != 0; iVar1 = iVar1 + -1) {
    *param_2 = *param_1;
    param_1 = param_1 + 1;
    param_2 = param_2 + 1;
  }
  return;
}
