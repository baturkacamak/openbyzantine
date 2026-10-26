// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x43bb77
// name: FUN_0043bb77
// ranges: 0x43bb77-0x43be08
// calls: 0x43bb77

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __cdecl FUN_0043bb77(int param_1,undefined4 *param_2)

{
  if ((*(byte *)(param_1 + 9) & 4) == 0) {
    if (*(short *)(param_1 + 6) == 0) {
      *(undefined2 *)(param_1 + 6) = *(undefined2 *)(param_1 + 4);
      *(undefined4 *)(param_1 + 0x20) = *(undefined4 *)(param_1 + 0x1c);
      if ((*(byte *)(param_1 + 9) & 2) == 0) {
        if (*(int *)(*(int *)(param_1 + 0x1c) + 4) == *(int *)(param_1 + 0x18)) {
          if (*(char *)(param_1 + 0x29) != '\0') {
            DAT_00482acd = 0;
            *(undefined1 *)(param_1 + 0x29) = 0;
          }
          if ((*(int *)(param_1 + 0x2c) != -1) && (param_2 != (undefined4 *)0x0)) {
            *param_2 = *(undefined4 *)(&DAT_004654e0 + *(int *)(param_1 + 0x2c) * 4);
            *(undefined4 *)(param_1 + 0x2c) = 0xffffffff;
          }
          if ((*(byte *)(param_1 + 9) & 8) != 0) {
            *(byte *)(param_1 + 9) = *(byte *)(param_1 + 9) | 4;
          }
          if ((*(byte *)(param_1 + 9) & 1) == 0) {
            if ((*(byte *)(param_1 + 9) & 4) == 0) {
              *(undefined4 *)(param_1 + 0x1c) = *(undefined4 *)(*(int *)(param_1 + 0x1c) + 4);
            }
          }
          else {
            *(undefined4 *)(param_1 + 0x1c) = *(undefined4 *)(*(int *)(param_1 + 0x1c) + 8);
            *(byte *)(param_1 + 9) = *(byte *)(param_1 + 9) | 2;
          }
        }
        else {
          *(undefined4 *)(param_1 + 0x1c) = *(undefined4 *)(*(int *)(param_1 + 0x1c) + 4);
        }
      }
      else if (*(int *)(*(int *)(param_1 + 0x1c) + 0x14) == 0) {
        if (*(char *)(param_1 + 0x29) != '\0') {
          DAT_00482acd = 0;
          *(undefined1 *)(param_1 + 0x29) = 0;
        }
        if ((*(int *)(param_1 + 0x2c) != -1) && (param_2 != (undefined4 *)0x0)) {
          *param_2 = *(undefined4 *)(&DAT_004654e0 + *(int *)(param_1 + 0x2c) * 4);
          *(undefined4 *)(param_1 + 0x2c) = 0xffffffff;
        }
        if ((*(byte *)(param_1 + 9) & 8) != 0) {
          *(byte *)(param_1 + 9) = *(byte *)(param_1 + 9) | 4;
        }
        if ((*(byte *)(param_1 + 9) & 1) == 0) {
          if ((*(byte *)(param_1 + 9) & 4) == 0) {
            *(undefined4 *)(param_1 + 0x1c) = *(undefined4 *)(*(int *)(param_1 + 0x1c) + 8);
          }
        }
        else {
          *(undefined4 *)(param_1 + 0x1c) = *(undefined4 *)(*(int *)(param_1 + 0x1c) + 4);
          *(byte *)(param_1 + 9) = *(byte *)(param_1 + 9) & 0xfd;
        }
      }
      else {
        *(undefined4 *)(param_1 + 0x1c) = *(undefined4 *)(*(int *)(param_1 + 0x1c) + 8);
      }
    }
    else {
      *(short *)(param_1 + 6) = *(short *)(param_1 + 6) - (short)_DAT_004859fc;
      if (0xfff0 < *(ushort *)(param_1 + 6)) {
        *(undefined2 *)(param_1 + 6) = 0;
      }
    }
  }
  else {
    *(undefined4 *)(param_1 + 0x20) = *(undefined4 *)(param_1 + 0x1c);
  }
  return;
}
