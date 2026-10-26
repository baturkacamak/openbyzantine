// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4119e3
// name: FUN_004119e3
// ranges: 0x4119e3-0x411b4a
// calls: 0x4119e3, 0x412762, 0x412aea

undefined4 __cdecl FUN_004119e3(undefined4 *param_1,FILE *param_2)

{
  uint uVar1;
  short sVar2;
  ushort uVar3;
  uint uVar4;
  byte local_10 [8];
  uint local_8;
  
  local_8 = local_8 & 0xffff0000;
  sVar2 = FUN_00412762(param_1,param_2);
  local_8._0_2_ = sVar2 + (short)local_8;
  sVar2 = FUN_00412762(param_1 + 1,param_2);
  local_8._0_2_ = sVar2 + (short)local_8;
  __fread_lk((char *)local_10,1,1,param_2);
  sVar2 = FUN_00412aea((int)(param_1 + 0x131),local_10[0],param_2);
  local_8 = CONCAT22(local_8._2_2_,sVar2 + (short)local_8);
  __fread_lk((char *)local_10,1,1,param_2);
  uVar3 = FUN_00412aea((int)(param_1 + 0x163),local_10[0],param_2);
  uVar4 = (uint)uVar3 + (local_8 & 0xffff);
  local_8 = uVar4 & 0xffff;
  uVar1 = local_8;
  local_8._0_2_ = (short)uVar4;
  if (uVar1 == 0) {
    *(undefined1 *)((int)param_1 + 0xe) = 0;
    *(undefined1 *)(param_1 + 3) = 0;
    *(undefined1 *)((int)param_1 + 0xd) = 0xff;
    *(undefined1 *)((int)param_1 + 0xb) = 0xff;
    for (uVar3 = 0; uVar4 = (uint)uVar3, uVar4 < 0x32; uVar3 = uVar3 + 1) {
      param_1[uVar3 + 0xf2] = param_1[uVar3 + 0x131];
      *(undefined1 *)(uVar3 + 0x490 + (int)param_1) = 0;
    }
  }
  return CONCAT22((short)(uVar4 >> 0x10),(short)local_8);
}
