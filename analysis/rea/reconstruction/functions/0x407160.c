// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x407160
// name: FUN_00407160
// ranges: 0x407160-0x407253
// calls: 0x407160, 0x40c624, 0x416710

undefined4 FUN_00407160(void)

{
  undefined4 uVar1;
  int local_114;
  uint local_110 [65];
  int local_c;
  undefined4 local_8;
  
  for (local_114 = 0; local_114 < 6; local_114 = local_114 + 1) {
    local_c = DAT_0045d1ac * local_114 + 0xa0;
    local_8 = 0x1aa;
    *(int *)(&DAT_00468d00 + local_114 * 0x10) = local_c;
    *(undefined4 *)(&DAT_00468d04 + local_114 * 0x10) = 0x1aa;
    *(int *)(&DAT_00468d08 + local_114 * 0x10) = DAT_0045d1ac + local_c;
    *(undefined4 *)(&DAT_00468d0c + local_114 * 0x10) = 0x1e6;
  }
  if (DAT_0045d1b0 == 0) {
    FUN_0040c624(local_110,(uint *)s_inventor_0045d1ec,(uint *)s_instrip_0045d1e4,
                 (uint *)&DAT_0045d1dc);
    uVar1 = FUN_00416710((char *)local_110,&DAT_0045d1b0,0,(uint *)0x0,(uint *)0x0);
  }
  else {
    uVar1 = 0;
  }
  return uVar1;
}
