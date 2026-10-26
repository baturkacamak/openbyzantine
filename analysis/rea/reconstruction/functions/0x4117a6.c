// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4117a6
// name: FUN_004117a6
// ranges: 0x4117a6-0x41194a
// calls: 0x4117a6, 0x41194b, 0x41d440, 0x4283a6, 0x4319b3, 0x436652

undefined4 FUN_004117a6(void)

{
  ushort uVar1;
  ushort uVar2;
  int iVar3;
  undefined1 local_4c;
  char *local_48;
  undefined4 local_44;
  uint local_40;
  uint local_3c;
  uint local_38;
  uint local_34;
  uint local_30;
  uint local_2c;
  uint local_28;
  
  local_44 = 0;
  FUN_0041194b();
  if (DAT_00460660 == 0) {
    for (local_48 = DAT_0047a238; local_48 != (char *)0x0; local_48 = *(char **)(local_48 + 0x2c)) {
      if (*local_48 == 'e') {
        uVar1 = FUN_00436652((int)local_48,0);
        if (local_48[7] == '\x02') {
          local_3c = (uint)*(ushort *)(local_48 + 0x22);
        }
        else {
          local_3c = 1;
        }
        uVar2 = FUN_00436652((int)local_48,5);
        local_28 = (uint)uVar2;
        if (((local_28 == 0) && (iVar3 = FUN_0041d440(2), iVar3 == 0)) && (local_3c == 0)) {
          local_3c = 1;
        }
        *(undefined1 *)(*(int *)(&DAT_0047a8e0 + (uint)uVar1 * 4) + 8) = (undefined1)local_3c;
        if (local_3c == 0) {
          local_40 = (uint)uVar1;
          local_2c = FUN_004319b3((int)local_48,1);
          local_2c = local_2c & 0xffff;
          local_30 = FUN_004319b3((int)local_48,2);
          local_30 = local_30 & 0xffff;
          uVar2 = FUN_00436652((int)local_48,3);
          local_34 = (uint)uVar2;
          uVar2 = FUN_00436652((int)local_48,4);
          local_38 = (uint)uVar2;
          iVar3 = FUN_004283a6((int *)&local_40,
                               (uint *)**(undefined4 **)(&DAT_0047a8e0 + (uint)uVar1 * 4));
          local_4c = (undefined1)iVar3;
          *(undefined1 *)(*(int *)(&DAT_0047a8e0 + (uint)uVar1 * 4) + 9) = local_4c;
          if (iVar3 == 0) {
            local_44 = 1;
          }
        }
      }
    }
  }
  else {
    local_44 = 0;
  }
  return local_44;
}
