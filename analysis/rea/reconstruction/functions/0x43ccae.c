// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x43ccae
// name: FUN_0043ccae
// ranges: 0x43ccae-0x43cd2b
// calls: 0x43ccae

uint __cdecl FUN_0043ccae(int param_1)

{
  int local_c;
  uint local_8;
  
  local_c = *(int *)(param_1 + 0x18);
  local_8 = 0;
  for (; ((int)local_8 < (int)(uint)*(ushort *)(param_1 + 0x14) &&
         (*(int *)(param_1 + 0x1c) != local_c)); local_c = *(int *)(local_c + 4)) {
    local_8 = local_8 + 1;
  }
  if (*(ushort *)(param_1 + 0x14) == local_8) {
    local_8 = 0xffffffff;
  }
  return local_8;
}
