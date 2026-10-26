// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x45a430
// name: FUN_0045a430
// ranges: 0x45a430-0x45a43f
// calls: 0x45a430

undefined4 __cdecl FUN_0045a430(undefined4 *param_1)

{
  undefined2 *puVar1;
  undefined2 *puVar2;
  
  puVar1 = (undefined2 *)*param_1;
  puVar2 = puVar1 + 2;
  *param_1 = puVar2;
  return CONCAT22((short)((uint)puVar2 >> 0x10),*puVar1);
}
