// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x40b718
// name: FUN_0040b718
// ranges: 0x40b718-0x40b911
// calls: 0x40b718

void __fastcall FUN_0040b718(int param_1)

{
  uint local_68;
  undefined1 local_64 [4];
  uint local_60;
  undefined4 local_5c;
  undefined1 local_58 [4];
  uint local_54;
  int local_50;
  undefined4 local_c;
  void *local_8;
  
  if ((DAT_0045da00 == 0) && (*(int *)(param_1 + 0x24) == 0)) {
    DAT_0045da00 = 1;
    (**(code **)(**(int **)(param_1 + 4) + 0x10))
              (*(undefined4 *)(param_1 + 4),param_1 + 0x40,local_58);
    if ((*(uint *)(param_1 + 0x40) < *(uint *)(param_1 + 0x38)) ||
       (*(int *)(param_1 + 0x38) + 0x15888U <= *(uint *)(param_1 + 0x40))) {
      *(undefined4 *)(param_1 + 0x3c) = *(undefined4 *)(param_1 + 0x40);
      local_50 = (**(code **)(**(int **)(param_1 + 4) + 0x2c))
                           (*(undefined4 *)(param_1 + 4),*(undefined4 *)(param_1 + 0x38),0x15888,
                            &local_8,&local_60,&local_c,local_64,0);
      if ((local_50 == 0) && (local_60 == 0x15888)) {
        if (*(int *)(param_1 + 0x28) == 0) {
          local_5c = AVIStreamRead(*(undefined4 *)(param_1 + 8),*(undefined4 *)(param_1 + 0x30),
                                   (int)(0x15888 / (ulonglong)*(uint *)(param_1 + 0x18)),local_8,
                                   0x15888,&local_54,&local_68);
        }
        else {
          _memset(local_8,0,0x15888);
          local_54 = local_60;
          local_68 = local_60 / *(uint *)(param_1 + 0x18);
        }
        *(int *)(param_1 + 0x30) = *(int *)(param_1 + 0x30) + local_68;
        *(int *)(param_1 + 0x34) = *(int *)(param_1 + 0x34) + 1;
        *(int *)(param_1 + 0x38) = *(int *)(param_1 + 0x38) + 0x15888;
        if (*(uint *)(param_1 + 0x14) <= *(uint *)(param_1 + 0x38)) {
          *(int *)(param_1 + 0x2c) = *(int *)(param_1 + 0x2c) + 1;
          *(int *)(param_1 + 0x38) = *(int *)(param_1 + 0x38) - *(int *)(param_1 + 0x14);
        }
        local_50 = (**(code **)(**(int **)(param_1 + 4) + 0x4c))
                             (*(undefined4 *)(param_1 + 4),local_8,local_54,local_c,0);
        if (*(uint *)(param_1 + 0x1c) <= *(uint *)(param_1 + 0x34)) {
          OutputDebugStringA(s______DSound__Timer_Done_Writing_0045da50);
          *(undefined4 *)(param_1 + 0x28) = 1;
        }
        DAT_0045da00 = 0;
      }
      else {
        DAT_0045da00 = 0;
      }
    }
    else {
      DAT_0045da00 = 0;
    }
  }
  return;
}
