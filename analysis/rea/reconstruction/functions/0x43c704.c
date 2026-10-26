// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x43c704
// name: FUN_0043c704
// ranges: 0x43c704-0x43c8d1
// calls: 0x41d3d5, 0x437c8c, 0x43c704, 0x44c580, 0x4599c0

uint __cdecl FUN_0043c704(undefined4 param_1,undefined4 param_2,int param_3,uint *param_4)

{
  uint uVar1;
  char *pcVar2;
  byte local_10;
  byte local_c;
  undefined1 local_8;
  
  FUN_00437c8c();
  *(undefined1 *)param_4 = 0;
  local_10 = 0;
  for (local_c = 0; (int)(uint)local_c < param_3; local_c = local_c + 1) {
    FUN_004599c0(&DAT_00461880);
  }
  do {
    FUN_00437c8c();
    uVar1 = FUN_0041d3d5();
    local_8 = (undefined1)uVar1;
    if ((((uVar1 < 0x61) || (0x7a < uVar1)) && ((uVar1 < 0x41 || (0x5a < uVar1)))) &&
       (((((uVar1 < 0x30 || (0x39 < uVar1)) && (uVar1 != 0x2e)) &&
         ((uVar1 != 0x24 && (uVar1 != 0x27)))) && (uVar1 != 0x3d)))) {
      if ((uVar1 == 8) && (local_10 != 0)) {
        local_10 = local_10 - 1;
        *(undefined1 *)((uint)local_10 + (int)param_4) = 0;
      }
    }
    else if ((int)(uint)local_10 < param_3) {
      *(undefined1 *)((uint)local_10 + (int)param_4) = local_8;
      local_10 = local_10 + 1;
    }
    FUN_00437c8c();
    *(undefined1 *)((uint)local_10 + (int)param_4) = 0;
    FUN_004599c0(&DAT_00461884);
    pcVar2 = FUN_0044c580(param_4);
    for (local_c = (byte)pcVar2; (int)(uint)local_c < param_3; local_c = local_c + 1) {
      FUN_004599c0(&DAT_00461888);
    }
    FUN_00437c8c();
  } while ((uVar1 != 0xd) && (uVar1 != 0x1b));
  if (uVar1 == 0xd) {
    *(undefined1 *)(param_3 + (int)param_4) = 0;
  }
  else {
    *(undefined1 *)param_4 = 0;
    param_3 = (int)param_4;
  }
  return CONCAT31((int3)((uint)param_3 >> 8),local_8);
}
