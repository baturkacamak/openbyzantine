// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x42bc8b
// name: FUN_0042bc8b
// ranges: 0x42bc8b-0x42bce0
// calls: 0x42bc8b

void __cdecl FUN_0042bc8b(undefined2 *param_1,undefined2 *param_2,int param_3,int param_4)

{
  undefined2 *puVar1;
  
  puVar1 = param_1 + 0x3e800;
  param_2 = param_2 + param_3;
  for (param_1 = param_1 + param_3; param_1 < puVar1; param_1 = param_1 + param_4) {
    *param_1 = *param_2;
    param_2 = param_2 + param_4;
  }
  return;
}
