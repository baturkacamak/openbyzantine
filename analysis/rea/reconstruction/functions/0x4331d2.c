// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4331d2
// name: FUN_004331d2
// ranges: 0x4331d2-0x433308
// calls: 0x4331d2

ushort __cdecl FUN_004331d2(int param_1,int param_2)

{
  bool bVar1;
  ushort uVar2;
  int local_14;
  int local_10;
  
  bVar1 = false;
  if (*(int *)(param_2 + (uint)*(ushort *)(param_1 + 8) * 4) == 0) {
    uVar2 = *(ushort *)(param_1 + 8);
  }
  else {
    local_10 = *(int *)(param_1 + 0x654 + (uint)*(ushort *)(param_1 + 8) * 4);
    local_14 = *(int *)(param_1 + 0x58c + (uint)*(ushort *)(param_1 + 8) * 4);
    while ((local_14 != 0 && (!bVar1))) {
      if (*(int *)(*(int *)(param_2 + (uint)*(ushort *)(param_1 + 8) * 4) + 4) == local_14) {
        local_14 = *(int *)(local_14 + 8);
      }
      else {
        if ((*(char *)(local_14 + 0x10) != '\0') && (*(char *)(local_14 + 0x11) == '\0')) {
          bVar1 = true;
        }
        local_14 = *(int *)(local_14 + 8);
        if ((local_14 == 0) && (local_10 != 0)) {
          local_14 = *(int *)(local_10 + 8);
          local_10 = *(int *)(local_10 + 4);
        }
      }
    }
    if (bVar1) {
      uVar2 = *(ushort *)(param_1 + 8);
    }
    else {
      uVar2 = (ushort)**(byte **)(param_2 + (uint)*(ushort *)(param_1 + 8) * 4);
    }
  }
  return uVar2;
}
