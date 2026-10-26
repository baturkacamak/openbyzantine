// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x401ca6
// name: FUN_00401ca6
// ranges: 0x401ca6-0x401d28
// calls: 0x401ca6, 0x4240ae

void __cdecl FUN_00401ca6(int *param_1,int *param_2)

{
  if ((((DAT_0045c060 == 0) || (DAT_00464880 == (void *)0x0)) || (param_1[1] < 0x14)) ||
     (0x1a3 < param_1[1])) {
    *param_2 = *param_1;
    param_2[1] = param_1[1];
  }
  else {
    param_1[1] = param_1[1] + -0x14;
    FUN_004240ae(DAT_00464880,param_1,param_2);
    param_2[1] = param_2[1] + 0x14;
  }
  return;
}
