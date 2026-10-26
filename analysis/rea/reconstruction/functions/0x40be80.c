// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x40be80
// name: FUN_0040be80
// ranges: 0x40be80-0x40bf4a; 0x40bf50-0x40bf63; 0x40bf74-0x40bf84
// calls: 0x40be80, 0x4164a0

void __cdecl FUN_0040be80(int param_1,int param_2)

{
  int iVar1;
  ushort uVar2;
  ushort uVar3;
  ushort *puVar4;
  int local_18;
  int local_10;
  int local_8;
  
  local_10 = 0;
  local_8 = -1;
  do {
    puVar4 = (ushort *)(local_10 * 2 + param_1);
    iVar1 = local_10 + 1;
    uVar3 = *puVar4;
    switch((byte)(*puVar4 >> 0xe)) {
    case 0:
      local_10 = iVar1;
      for (local_18 = 0; iVar1 = local_10, local_18 < (short)(uVar3 & 0x3fff);
          local_18 = local_18 + 1) {
        uVar2 = FUN_004164a0(*(ushort *)(param_1 + local_10 * 2));
        *(ushort *)(param_1 + local_10 * 2) = uVar2;
        local_10 = local_10 + 1;
      }
      break;
    case 1:
      local_8 = local_8 + 1;
      break;
    case 2:
      uVar3 = FUN_004164a0(*(ushort *)(param_1 + iVar1 * 2));
      *(ushort *)(param_1 + iVar1 * 2) = uVar3;
      iVar1 = local_10 + 2;
      break;
    case 3:
    }
    local_10 = iVar1;
  } while (local_8 < param_2);
  return;
}
