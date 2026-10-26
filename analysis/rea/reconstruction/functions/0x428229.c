// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x428229
// name: FUN_00428229
// ranges: 0x428229-0x42826d
// calls: 0x428229, 0x44c2f0

undefined4 * __cdecl FUN_00428229(undefined4 param_1,undefined4 param_2)

{
  undefined4 *puVar1;
  
  puVar1 = (undefined4 *)FUN_0044c2f0(0xc);
  puVar1[2] = param_1;
  *puVar1 = param_2;
  *(undefined1 *)((int)puVar1 + 5) = 1;
  return puVar1;
}
