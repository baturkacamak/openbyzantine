// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x40f98c
// name: FUN_0040f98c
// ranges: 0x40f98c-0x40fbf5
// calls: 0x40f98c, 0x413729, 0x413802, 0x41383a, 0x44ac40

void __cdecl FUN_0040f98c(int param_1)

{
  int local_8;
  
  if (*(int *)(&DAT_00482ad0 + param_1 * 4) != 0) {
    FUN_00413802(*(LPVOID *)(*(int *)(&DAT_00482ad0 + param_1 * 4) + 4));
    *(undefined4 *)(*(int *)(&DAT_00482ad0 + param_1 * 4) + 4) = 0;
    FUN_00413802((LPVOID)**(undefined4 **)(&DAT_00482ad0 + param_1 * 4));
    **(undefined4 **)(&DAT_00482ad0 + param_1 * 4) = 0;
    FUN_00413729(*(int *)(&DAT_00482ad0 + param_1 * 4) + 0x4c4);
    FUN_00413729(*(int *)(&DAT_00482ad0 + param_1 * 4) + 0x58c);
    FUN_0041383a(*(int *)(&DAT_00482ad0 + param_1 * 4) + 0x654);
    for (local_8 = 0; local_8 < 0x32; local_8 = local_8 + 1) {
      if (*(int *)(*(int *)(&DAT_00482ad0 + param_1 * 4) + 0xa8 + local_8 * 4) != 0) {
        FUN_0044ac40(*(LPVOID *)(*(int *)(&DAT_00482ad0 + param_1 * 4) + 0xa8 + local_8 * 4));
      }
      if (*(int *)(*(int *)(&DAT_00482ad0 + param_1 * 4) + 0x170 + local_8 * 4) != 0) {
        FUN_0044ac40(*(LPVOID *)(*(int *)(&DAT_00482ad0 + param_1 * 4) + 0x170 + local_8 * 4));
      }
      if (*(int *)(*(int *)(&DAT_00482ad0 + param_1 * 4) + 0x238 + local_8 * 4) != 0) {
        FUN_0044ac40(*(LPVOID *)(*(int *)(&DAT_00482ad0 + param_1 * 4) + 0x238 + local_8 * 4));
      }
    }
    for (local_8 = 0;
        local_8 < (int)(uint)*(ushort *)(*(int *)(&DAT_00482ad0 + param_1 * 4) + 0x71c);
        local_8 = local_8 + 1) {
      if (*(int *)(*(int *)(&DAT_00482ad0 + param_1 * 4) + 0x720 + local_8 * 4) != 0) {
        FUN_0044ac40(*(LPVOID *)(*(int *)(&DAT_00482ad0 + param_1 * 4) + 0x720 + local_8 * 4));
      }
    }
    for (local_8 = 0;
        local_8 < (int)(uint)*(ushort *)(*(int *)(&DAT_00482ad0 + param_1 * 4) + 0x12d8);
        local_8 = local_8 + 1) {
      if (*(int *)(*(int *)(&DAT_00482ad0 + param_1 * 4) + 0x12dc + local_8 * 4) != 0) {
        FUN_0044ac40(*(LPVOID *)(*(int *)(&DAT_00482ad0 + param_1 * 4) + 0x12dc + local_8 * 4));
      }
    }
    FUN_0044ac40(*(LPVOID *)(&DAT_00482ad0 + param_1 * 4));
    *(undefined4 *)(&DAT_00482ad0 + param_1 * 4) = 0;
  }
  return;
}
