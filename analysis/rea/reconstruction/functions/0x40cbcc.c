// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x40cbcc
// name: FUN_0040cbcc
// ranges: 0x40cbcc-0x40cd75
// calls: 0x40cbcc, 0x4180f4, 0x426f95, 0x43b216

void __cdecl FUN_0040cbcc(int param_1,char param_2,int param_3)

{
  ushort uVar1;
  int unaff_EDI;
  byte abStack_608 [768];
  uint local_308;
  char local_304 [768];
  
  local_308 = local_308 & 0xffff0000;
  while ((local_308 & 0xffff) < 0x300) {
    abStack_608[local_308 & 0xffff] = *(byte *)((local_308 & 0xffff) + param_1);
    abStack_608[local_308 & 0xffff] =
         (byte)((longlong)(ulonglong)abStack_608[local_308 & 0xffff] / (longlong)param_3);
    local_304[local_308 & 0xffff] = *(char *)((local_308 & 0xffff) + param_1);
    local_308 = CONCAT22(local_308._2_2_,(short)local_308 + 1);
  }
  local_308 = local_308 & 0xffff0000;
  do {
    if (param_2 != '\0') {
      FUN_0043b216((undefined4 *)0x0,0,unaff_EDI);
    }
    for (uVar1 = 0; uVar1 < 0x300; uVar1 = uVar1 + 1) {
      local_304[uVar1] = local_304[uVar1] - abStack_608[uVar1];
    }
    FUN_004180f4((int)local_304);
    FUN_00426f95(0x32);
    uVar1 = (short)local_308 + 1;
    local_308 = CONCAT22(local_308._2_2_,uVar1);
  } while ((int)(uint)uVar1 < param_3);
  _memset(local_304,0,0x300);
  FUN_004180f4((int)local_304);
  return;
}
