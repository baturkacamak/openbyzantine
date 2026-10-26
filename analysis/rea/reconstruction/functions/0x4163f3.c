// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4163f3
// name: FUN_004163f3
// ranges: 0x4163f3-0x41649d
// calls: 0x40c624, 0x40d3af, 0x4163f3, 0x41842b, 0x42b280

void FUN_004163f3(void)

{
  int unaff_EDI;
  uint local_108 [65];
  
  DAT_0045f050 = 0;
  FUN_0042b280(DAT_0046e82c,0x4b000);
  FUN_0041842b((int)DAT_0046e82c,0x4b000);
  FUN_0040c624(local_108,(uint *)s_video_0045f1c8,(uint *)&DAT_0045f1c4,(uint *)&DAT_0045f1bc);
  FUN_0040d3af(local_108,6,0,0x14,0,unaff_EDI);
  FUN_0040c624(local_108,(uint *)s_video_0045f1dc,(uint *)&DAT_0045f1d8,(uint *)&DAT_0045f1d0);
  FUN_0040d3af(local_108,6,0,0,0,unaff_EDI);
  return;
}
