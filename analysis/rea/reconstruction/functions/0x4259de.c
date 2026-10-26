// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4259de
// name: FUN_004259de
// ranges: 0x4259de-0x425a53
// calls: 0x4259de, 0x44a460

void __cdecl FUN_004259de(int param_1,FILE *param_2)

{
  undefined4 local_c;
  undefined4 local_8;
  
  local_8 = 0xffff;
  local_c = 0xffff;
  if (*(int *)(param_1 + 0x1c) != 0) {
    local_8 = *(undefined4 *)(*(int *)(param_1 + 0x1c) + 0x14);
  }
  if (*(int *)(param_1 + 0x20) != 0) {
    local_c = *(undefined4 *)(*(int *)(param_1 + 0x20) + 0x14);
  }
  FUN_0044a460((char *)&local_8,4,1,param_2);
  FUN_0044a460((char *)&local_c,4,1,param_2);
  return;
}
