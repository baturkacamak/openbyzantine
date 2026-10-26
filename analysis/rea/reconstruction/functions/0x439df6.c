// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x439df6
// name: FUN_00439df6
// ranges: 0x439df6-0x43a03a
// calls: 0x439df6, 0x43a03b, 0x44a820, 0x44ac60

undefined4 __fastcall FUN_00439df6(int param_1)

{
  int iVar1;
  undefined4 uVar2;
  int local_94;
  undefined1 local_90 [4];
  undefined4 local_8c;
  undefined4 local_68;
  
  iVar1 = AVIFileGetStream(*(undefined4 *)(param_1 + 0x14),param_1 + 0x28,0x73646976,0);
  if (iVar1 == 0) {
    uVar2 = AVIStreamLength(*(undefined4 *)(param_1 + 0x28));
    *(undefined4 *)(param_1 + 0x34) = uVar2;
    if (*(int *)(param_1 + 0x34) == -1) {
      FUN_0043a03b(param_1);
      uVar2 = 0xffffffff;
    }
    else {
      *(int *)(param_1 + 0x80) = *(int *)(param_1 + 0x34) + -1;
      iVar1 = AVIStreamInfoA(*(undefined4 *)(param_1 + 0x28),local_90,0x8c);
      if (iVar1 == 0) {
        *(undefined4 *)(param_1 + 0x38) = 0x42;
        *(undefined4 *)(param_1 + 0x3c) = local_68;
        *(undefined4 *)(param_1 + 0x40) = local_8c;
        if (*(int *)(param_1 + 0x40) != 0x31345649) {
          OutputDebugStringA(s_Init_Video_for_Windows_004617fc);
        }
        iVar1 = AVIStreamReadFormat(*(undefined4 *)(param_1 + 0x28),0,0,&local_94);
        if (iVar1 == 0) {
          if (local_94 == 0x28) {
            if (*(int *)(param_1 + 0x54) != 0) {
              FUN_0044a820(*(LPVOID *)(param_1 + 0x54));
            }
            uVar2 = FUN_0044ac60(0x28);
            *(undefined4 *)(param_1 + 0x54) = uVar2;
            if (*(int *)(param_1 + 0x54) == 0) {
              FUN_0043a03b(param_1);
              uVar2 = 0xffffffff;
            }
            else {
              AVIStreamReadFormat(*(undefined4 *)(param_1 + 0x28),0,0,&local_94);
              iVar1 = AVIStreamReadFormat(*(undefined4 *)(param_1 + 0x28),0,
                                          *(undefined4 *)(param_1 + 0x54),&local_94);
              if (iVar1 == 0) {
                uVar2 = 0;
              }
              else {
                FUN_0043a03b(param_1);
                uVar2 = 0xffffffff;
              }
            }
          }
          else {
            FUN_0043a03b(param_1);
            uVar2 = 0xffffffff;
          }
        }
        else {
          FUN_0043a03b(param_1);
          uVar2 = 0xffffffff;
        }
      }
      else {
        FUN_0043a03b(param_1);
        uVar2 = 0xffffffff;
      }
    }
  }
  else {
    uVar2 = 0xffffffff;
  }
  return uVar2;
}
