// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4087c1
// name: FUN_004087c1
// ranges: 0x4087c1-0x40885a
// calls: 0x407160, 0x407420, 0x4087c1, 0x40c624, 0x416974

void __cdecl FUN_004087c1(int param_1)

{
  uint local_108 [65];
  
  if (param_1 == -1) {
    FUN_0040c624(local_108,(uint *)s_inventor_0045d254,(uint *)s_instrip_0045d24c,
                 (uint *)&DAT_0045d244);
    DAT_0045d1ac = 0x3c;
  }
  else {
    FUN_0040c624(local_108,(uint *)s_inventor_0045d274,(uint *)s_klinstrp_0045d268,
                 (uint *)&DAT_0045d260);
    DAT_0045d1ac = 0x42;
  }
  FUN_00416974((char *)local_108,DAT_0045d1b4,0,0,0);
  FUN_00407160();
  FUN_00407420(0);
  return;
}
