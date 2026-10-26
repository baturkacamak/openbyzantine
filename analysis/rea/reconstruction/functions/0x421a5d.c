// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x421a5d
// name: FUN_00421a5d
// ranges: 0x421a5d-0x421b8b
// calls: 0x40701f, 0x40711b, 0x421a5d, 0x44c520

int __cdecl FUN_00421a5d(int param_1,byte param_2)

{
  uint *puVar1;
  int iVar2;
  int local_20;
  int local_1c;
  int local_14;
  uint *local_10;
  int local_8;
  
  if ((param_2 & 1) == 0) {
    local_14 = 0x61;
    local_8 = 0x165;
  }
  else {
    local_14 = 0x69;
    local_8 = 0x1b3;
  }
  local_1c = 0;
  while( true ) {
    if (*(int *)(param_1 + 4) <= local_1c) {
      return local_1c;
    }
    puVar1 = *(uint **)(*(int *)(param_1 + 0x10) + local_1c * 4);
    iVar2 = FUN_0040701f(puVar1,0xb9,2);
    local_10 = puVar1;
    for (local_20 = 0; (local_20 < iVar2 && (local_20 * 0x14 + local_14 + 0x14 <= local_8));
        local_20 = local_20 + 1) {
      local_10 = (uint *)FUN_0044c520((char *)((int)local_10 + 1),'\n');
    }
    FUN_0040711b((char *)puVar1);
    if (local_20 < iVar2) break;
    local_14 = local_14 + 0x14 + iVar2 * 0x14;
    local_1c = local_1c + 1;
  }
  if (local_20 < 1) {
    return local_1c;
  }
  if (local_10 == puVar1) {
    return local_1c;
  }
  *(undefined1 *)local_10 = 10;
  return local_1c + 1;
}
