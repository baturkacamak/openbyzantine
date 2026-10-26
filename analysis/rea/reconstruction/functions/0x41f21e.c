// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x41f21e
// name: FUN_0041f21e
// ranges: 0x41f21e-0x41f278
// calls: 0x41f21e, 0x44cc60

bool FUN_0041f21e(void)

{
  bool bVar1;
  DWORD local_120 [70];
  HANDLE local_8;
  
  local_8 = __findfirst(s_savegame___btb_0045fa5c,local_120);
  bVar1 = local_8 != (HANDLE)0xffffffff;
  if (bVar1) {
    FUN_0044cc60(local_8);
  }
  return bVar1;
}
