// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x45a400
// name: FUN_0045a400
// ranges: 0x45a400-0x45a40e
// calls: 0x45a400

undefined4 __cdecl FUN_0045a400(int *param_1)

{
  undefined4 *puVar1;
  
  puVar1 = (undefined4 *)*param_1;
  *param_1 = (int)(puVar1 + 1);
  return *puVar1;
}
