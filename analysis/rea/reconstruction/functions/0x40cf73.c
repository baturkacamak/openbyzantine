// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x40cf73
// name: FUN_0040cf73
// ranges: 0x40cf73-0x40d030
// calls: 0x40ac38, 0x40ac43, 0x40cf73

undefined4 __cdecl FUN_0040cf73(int param_1,int param_2)

{
  undefined4 local_c;
  
  if (param_2 == 0xffff) {
    *(undefined4 *)(param_1 + 0x1c) = 0;
  }
  else {
    *(undefined4 *)(param_1 + 0x1c) = *(undefined4 *)(param_1 + 0x18);
    for (local_c = 0; local_c < param_2; local_c = local_c + 1) {
      if (*(int *)(*(int *)(param_1 + 0x1c) + 4) == 0) {
        FUN_0040ac43();
        FUN_0040ac38();
      }
      else {
        *(undefined4 *)(param_1 + 0x1c) = *(undefined4 *)(*(int *)(param_1 + 0x1c) + 4);
      }
    }
  }
  return 0;
}
