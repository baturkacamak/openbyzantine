// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x418cd0
// name: FUN_00418cd0
// ranges: 0x418cd0-0x418de8
// calls: 0x4066c6, 0x41842b, 0x418cd0

void __cdecl FUN_00418cd0(ushort *param_1,ushort *param_2)

{
  undefined2 *_Src;
  undefined4 uVar1;
  int local_24;
  int local_20;
  int local_1c;
  ushort *local_18;
  ushort *local_14;
  undefined2 *local_10;
  byte local_c;
  
  local_c = 1;
  for (local_24 = 0; local_24 < 8; local_24 = local_24 + 1) {
    local_14 = param_1;
    local_18 = param_2;
    local_10 = (undefined2 *)(DAT_00477344 + 0x6400);
    for (local_1c = 0; _Src = local_10, local_1c < 200; local_1c = local_1c + 1) {
      for (local_20 = 0; local_20 < 0x140; local_20 = local_20 + 1) {
        uVar1 = FUN_004066c6((uint)*local_18,(uint)*local_14,(uint)local_c);
        *local_10 = (short)uVar1;
        local_10[1] = *local_10;
        local_10 = local_10 + 2;
        local_14 = local_14 + 2;
        local_18 = local_18 + 2;
      }
      local_14 = local_14 + 0x280;
      local_18 = local_18 + 0x280;
      FID_conflict__memcpy(local_10,_Src,0x500);
      local_10 = local_10 + 0x280;
    }
    FUN_0041842b(DAT_00477344,0x3e800);
    local_c = local_c * '\x02' | 1;
  }
  return;
}
