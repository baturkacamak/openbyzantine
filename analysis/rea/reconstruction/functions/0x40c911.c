// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x40c911
// name: FUN_0040c911
// ranges: 0x40c911-0x40c9cd
// calls: 0x40c911

int FUN_0040c911(void)

{
  DWORD DVar1;
  UINT UVar2;
  int local_20;
  int local_14;
  char local_10 [12];
  
  local_14 = 0;
  DAT_0045da7c = 0;
  DVar1 = GetLogicalDrives();
  local_20 = 0x44;
  do {
    if (0x5a < local_20) {
      return local_14;
    }
    local_10[0] = (char)local_20;
    if ((DVar1 & 1 << (local_10[0] + 0xbfU & 0x1f)) != 0) {
      local_10[1] = 0x3a;
      local_10[2] = 0x5c;
      local_10[3] = 0;
      UVar2 = GetDriveTypeA(local_10);
      if (UVar2 == 5) {
        DAT_0045da7c = DAT_0045da7c + 1;
        if (local_14 == 0) {
          local_14 = local_20;
        }
      }
      else if (local_14 != 0) {
        return local_14;
      }
    }
    local_20 = local_20 + 1;
  } while( true );
}
