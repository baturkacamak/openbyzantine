// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4330bf
// name: FUN_004330bf
// ranges: 0x4330bf-0x4331d1
// calls: 0x4330bf

ushort __cdecl FUN_004330bf(int param_1,int param_2)

{
  bool bVar1;
  ushort uVar2;
  int local_14;
  int local_10;
  
  bVar1 = false;
  if (*(char *)((uint)*(ushort *)(param_1 + 8) + param_2) == -1) {
    uVar2 = *(ushort *)(param_1 + 8);
  }
  else {
    local_10 = *(int *)(param_1 + 0x654 + (uint)*(ushort *)(param_1 + 8) * 4);
    local_14 = *(int *)(param_1 + 0x58c + (uint)*(ushort *)(param_1 + 8) * 4);
    while ((local_14 != 0 && (!bVar1))) {
      if ((*(char *)(local_14 + 0x10) != '\0') && (*(char *)(local_14 + 0x11) == '\0')) {
        bVar1 = true;
      }
      local_14 = *(int *)(local_14 + 8);
      if ((local_14 == 0) && (local_10 != 0)) {
        local_14 = *(int *)(local_10 + 8);
        local_10 = *(int *)(local_10 + 4);
      }
    }
    if (bVar1) {
      uVar2 = *(ushort *)(param_1 + 8);
    }
    else {
      uVar2 = (ushort)*(byte *)((uint)*(ushort *)(param_1 + 8) + param_2);
    }
  }
  return uVar2;
}
