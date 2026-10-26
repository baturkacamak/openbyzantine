// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x42130c
// name: FUN_0042130c
// ranges: 0x42130c-0x421372
// calls: 0x42130c, 0x44c2f0

undefined4 * __cdecl FUN_0042130c(undefined4 param_1,int param_2,undefined4 param_3)

{
  undefined4 *puVar1;
  undefined4 uVar2;
  
  puVar1 = (undefined4 *)FUN_0044c2f0(0x14);
  *puVar1 = param_1;
  puVar1[1] = param_2;
  puVar1[2] = param_3;
  uVar2 = FUN_0044c2f0(param_2 << 2);
  puVar1[3] = uVar2;
  uVar2 = FUN_0044c2f0(param_2 << 2);
  puVar1[4] = uVar2;
  return puVar1;
}
