// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x418a1e
// name: FUN_00418a1e
// ranges: 0x418a1e-0x418acc
// calls: 0x418040, 0x418a1e, 0x42b29f

void FUN_00418a1e(void)

{
  uint local_1c;
  uint local_18;
  undefined4 local_14;
  int local_10;
  void *local_c;
  int *local_8;
  
  local_c = DAT_00477344;
  local_8 = (int *)FUN_00418040((int)DAT_00477344);
  FUN_0042b29f((int)DAT_00477344,DAT_00485c88,0x4b000);
  local_1c = (uint)DAT_00485c88;
  local_18 = (uint)DAT_00485c88;
  local_14 = (**(code **)(*local_8 + 0x74))(local_8,8,&local_1c);
  for (local_10 = 0; local_10 < 0x1e0; local_10 = local_10 + 2) {
    _memset(local_c,0,0x500);
    local_c = (void *)((int)local_c + 0xa00);
  }
  return;
}
