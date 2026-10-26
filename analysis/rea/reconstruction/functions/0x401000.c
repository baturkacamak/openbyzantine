// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x401000
// name: FUN_00401000
// ranges: 0x401000-0x40105c
// calls: 0x401000, 0x4240ae, 0x44a460

void __cdecl FUN_00401000(FILE *param_1)

{
  int local_14 [2];
  int local_c [2];
  
  local_c[0] = 0x140;
  local_c[1] = 0;
  FUN_004240ae(DAT_00464880,local_c,local_14);
  FUN_0044a460((char *)&DAT_00480330,4,1,param_1);
  FUN_0044a460((char *)local_14,4,1,param_1);
  return;
}
