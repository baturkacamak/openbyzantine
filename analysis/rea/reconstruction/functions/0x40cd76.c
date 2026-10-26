// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x40cd76
// name: FUN_0040cd76
// ranges: 0x40cd76-0x40cec8
// calls: 0x40cd76, 0x4180f4, 0x426f95, 0x43b216

void __cdecl FUN_0040cd76(int param_1,char param_2,int param_3)

{
  ushort uVar1;
  int unaff_EDI;
  byte abStack_608 [768];
  uint local_308;
  char local_304 [768];
  
  local_308 = local_308 & 0xffff0000;
  while ((local_308 & 0xffff) < 0x300) {
    abStack_608[local_308 & 0xffff] = 0;
    local_308 = CONCAT22(local_308._2_2_,(short)local_308 + 1);
  }
  local_308 = local_308 & 0xffff0000;
  do {
    if (param_2 != '\0') {
      FUN_0043b216((undefined4 *)0x0,0,unaff_EDI);
    }
    for (uVar1 = 0; uVar1 < 0x300; uVar1 = uVar1 + 1) {
      abStack_608[uVar1] = *(byte *)((uint)uVar1 + param_1);
      local_304[uVar1] =
           (char)((longlong)(ulonglong)abStack_608[uVar1] / (longlong)param_3) * (char)local_308;
    }
    FUN_004180f4((int)local_304);
    FUN_00426f95(0x32);
    uVar1 = (short)local_308 + 1;
    local_308 = CONCAT22(local_308._2_2_,uVar1);
  } while ((int)(uint)uVar1 < param_3);
  FUN_004180f4(param_1);
  return;
}
