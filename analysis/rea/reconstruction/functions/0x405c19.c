// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x405c19
// name: FUN_00405c19
// ranges: 0x405c19-0x405db1
// calls: 0x405c19

void __cdecl FUN_00405c19(int param_1,ushort *param_2)

{
  ushort *puVar1;
  int iVar2;
  size_t _Size;
  int local_2c;
  void *local_28;
  int local_24;
  void *local_18;
  
  puVar1 = param_2;
  if (*(char *)(param_1 + 0x10) == '\x10') {
    for (local_24 = 0; local_24 < (int)(uint)*(ushort *)(param_1 + 0xe); local_24 = local_24 + 1) {
      for (local_2c = 0; local_2c < (int)(uint)*(ushort *)(param_1 + 0xc); local_2c = local_2c + 1)
      {
        *param_2 = ((*param_2 & 0x7c00) >> 10) << (DAT_00485c78 & 0x1f) |
                   ((*param_2 & 0x3e0) >> 5) << (DAT_00485c80 & 0x1f) | *param_2 & 0x1f;
        param_2 = param_2 + 1;
      }
    }
  }
  local_28 = *(void **)(param_1 + 0x14);
  iVar2 = (*(ushort *)(param_1 + 0xe) - 1) * (uint)*(byte *)(param_1 + 0x10) *
          (uint)*(ushort *)(param_1 + 0xc);
  local_18 = (void *)((int)puVar1 + ((int)(iVar2 + (iVar2 >> 0x1f & 7U)) >> 3));
  _Size = ((int)(uint)*(byte *)(param_1 + 0x10) >> 3) * (uint)*(ushort *)(param_1 + 0xc);
  for (local_24 = 0; local_24 < (int)(uint)*(ushort *)(param_1 + 0xe); local_24 = local_24 + 1) {
    FID_conflict__memcpy(local_28,local_18,_Size);
    local_28 = (void *)((int)local_28 + _Size);
    local_18 = (void *)((int)local_18 - _Size);
  }
  return;
}
