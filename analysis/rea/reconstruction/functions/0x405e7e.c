// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x405e7e
// name: FUN_00405e7e
// ranges: 0x405e7e-0x406040
// calls: 0x405c19, 0x405e7e, 0x44ac40, 0x44c2f0

void __cdecl FUN_00405e7e(int param_1,int param_2,undefined2 param_3,undefined2 param_4)

{
  undefined1 local_1c [12];
  undefined4 local_10;
  undefined1 local_c;
  void *local_8;
  
  *(int *)(&DAT_00487510 + param_2 * 0x48) = param_1;
  (&DAT_00487518)[param_2 * 0x12] =
       *(int *)(*(int *)(&DAT_00487510 + param_2 * 0x48) + 0x10) + param_1;
  if (**(short **)(&DAT_00487510 + param_2 * 0x48) == 8) {
    *(int *)(&DAT_00487514 + param_2 * 0x48) =
         *(int *)(*(int *)(&DAT_00487510 + param_2 * 0x48) + 4) + param_1;
    *(int *)(&DAT_0048751c + param_2 * 0x48) =
         *(int *)(*(int *)(&DAT_00487510 + param_2 * 0x48) + 0x14) + param_1;
  }
  else {
    *(int *)(&DAT_00487520 + param_2 * 0x48) =
         *(int *)(*(int *)(&DAT_00487510 + param_2 * 0x48) + 0x14) + param_1;
    local_10 = CONCAT22(*(ushort *)(*(int *)(&DAT_00487510 + param_2 * 0x48) + 0xc),
                        *(ushort *)(*(int *)(&DAT_00487510 + param_2 * 0x48) + 10));
    local_c = 0x10;
    if (((param_2 == 3) || (param_2 == 5)) || (param_2 == 6)) {
      local_8 = (void *)FUN_0044c2f0((uint)*(ushort *)
                                            (*(int *)(&DAT_00487510 + param_2 * 0x48) + 10) *
                                     (uint)*(ushort *)
                                            (*(int *)(&DAT_00487510 + param_2 * 0x48) + 0xc) * 2);
      FUN_00405c19((int)local_1c,*(ushort **)(&DAT_00487520 + param_2 * 0x48));
      FID_conflict__memcpy
                (*(void **)(&DAT_00487520 + param_2 * 0x48),local_8,
                 (local_10 & 0xffff) * (local_10 >> 0x10) * 2);
      FUN_0044ac40(local_8);
    }
  }
  *(undefined2 *)(&DAT_00487554 + param_2 * 0x48) = param_3;
  *(undefined2 *)(&DAT_00487556 + param_2 * 0x48) = param_4;
  *(undefined4 *)(&DAT_00487538 + param_2 * 0x48) = 0;
  *(undefined4 *)(&DAT_00487550 + param_2 * 0x48) = 0;
  return;
}
