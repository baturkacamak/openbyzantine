// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4241e2
// name: FUN_004241e2
// ranges: 0x4241e2-0x42423c
// calls: 0x423a37, 0x4241e2
// port: Projection reset [partial]

undefined8 __fastcall FUN_004241e2(int param_1,undefined4 param_2)

{
  undefined8 uVar1;
  
  *(undefined4 *)(param_1 + 0x50) = 0x54442eea;
  *(undefined4 *)(param_1 + 0x54) = 0x400921fb;
  *(undefined4 *)(param_1 + 0x58) = 0;
  *(undefined4 *)(param_1 + 0x5c) = 0;
  *(undefined4 *)(param_1 + 0x60) = 0;
  *(undefined4 *)(param_1 + 100) = 0x3ff00000;
  *(undefined4 *)(param_1 + 0x68) = 0;
  uVar1 = FUN_00423a37(param_1,param_2);
  return uVar1;
}
