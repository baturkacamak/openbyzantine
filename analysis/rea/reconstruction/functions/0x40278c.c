// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x40278c
// name: FUN_0040278c
// ranges: 0x40278c-0x402807
// calls: 0x40278c, 0x4058fd, 0x428f8c

void FUN_0040278c(void)

{
  uint local_10c [65];
  uint local_8;
  
  if (DAT_0045c058 == (int *)0x0) {
    FUN_00428f8c(local_10c,(uint *)s_simfrtz2_0045c0f0,(uint *)&DAT_0045c0e8);
    FUN_004058fd((LPSTR)local_10c,&local_8,&DAT_0045c058);
    if (DAT_0045c058 == (int *)0x0) {
      return;
    }
  }
  (**(code **)(*DAT_0045c058 + 0x30))(DAT_0045c058,0,0,0);
  return;
}
