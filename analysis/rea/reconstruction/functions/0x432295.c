// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x432295
// name: FUN_00432295
// ranges: 0x432295-0x432302
// calls: 0x40186e, 0x401c63, 0x4075fe, 0x40da1f, 0x432295

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __cdecl FUN_00432295(int param_1,int param_2,int param_3,int param_4)

{
  float10 fVar1;
  
  if (0 < param_3) {
    fVar1 = (float10)FUN_00401c63();
    _DAT_0047a8dc = (float)fVar1;
  }
  DAT_00480330 = param_1;
  DAT_0045c060 = 1;
  FUN_0040186e(**(undefined4 **)(&DAT_00483d90 + param_1 * 4),param_2,param_4);
  FUN_0040da1f();
  FUN_004075fe(1);
  DAT_004859cd = 0;
  return;
}
