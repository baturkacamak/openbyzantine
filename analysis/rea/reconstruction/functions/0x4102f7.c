// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4102f7
// name: FUN_004102f7
// ranges: 0x4102f7-0x41045a
// calls: 0x4102f7, 0x41cd4b, 0x432602, 0x44ac40

void __cdecl FUN_004102f7(int param_1)

{
  int *piVar1;
  LPVOID pvVar2;
  int *local_14;
  LPVOID local_8;
  
  if (*(int *)(&DAT_0047dd70 + param_1 * 4) != 0) {
    *(undefined1 *)(*(int *)(&DAT_0047dd70 + param_1 * 4) + 9) = 0;
  }
  if (*(int *)(&DAT_004649e8 + param_1 * 4) != 0) {
    local_14 = *(int **)(*(int *)(&DAT_004649e8 + param_1 * 4) + 0x18);
    if ((local_14 != (int *)0x0) && (local_14[2] != 0)) {
      *(undefined4 *)(local_14[2] + 4) = 0;
    }
    while (local_14 != (int *)0x0) {
      if (*local_14 != 0) {
        FUN_0044ac40((LPVOID)*local_14);
      }
      piVar1 = (int *)local_14[1];
      FUN_0044ac40(local_14);
      local_14 = piVar1;
    }
    local_8 = *(LPVOID *)(*(int *)(&DAT_004649e8 + param_1 * 4) + 0x34);
    while (local_8 != (LPVOID)0x0) {
      pvVar2 = *(LPVOID *)((int)local_8 + 8);
      FUN_0044ac40(local_8);
      local_8 = pvVar2;
    }
    if (*(int *)(*(int *)(&DAT_004649e8 + param_1 * 4) + 0x38) != 0) {
      FUN_0041cd4b(*(int *)(&DAT_004649e8 + param_1 * 4));
    }
    if (*(int *)(*(int *)(&DAT_004649e8 + param_1 * 4) + 0x3c) != 0) {
      FUN_00432602(*(int *)(&DAT_004649e8 + param_1 * 4));
    }
    FUN_0044ac40(*(LPVOID *)(&DAT_004649e8 + param_1 * 4));
    *(undefined4 *)(&DAT_004649e8 + param_1 * 4) = 0;
  }
  return;
}
