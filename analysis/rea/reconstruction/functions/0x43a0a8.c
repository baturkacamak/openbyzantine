// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x43a0a8
// name: FUN_0043a0a8
// ranges: 0x43a0a8-0x43a20c
// calls: 0x43a0a8, 0x43a20d

undefined4 __fastcall FUN_0043a0a8(undefined4 *param_1)

{
  int iVar1;
  undefined4 uVar2;
  longlong lVar3;
  undefined1 local_90 [32];
  undefined4 local_70;
  undefined4 local_6c;
  undefined4 local_60;
  
  iVar1 = AVIFileGetStream(param_1[5],param_1 + 0x18,0x73647561,0);
  if (iVar1 == 0) {
    iVar1 = AVIStreamInfoA(param_1[0x18],local_90,0x8c);
    if (iVar1 == 0) {
      param_1[0x1c] = local_6c;
      if (param_1[0x1c] == 0) {
        MessageBoxA((HWND)*param_1,s_No_Audio_Skew_on_File_00461828,(LPCSTR)0x0,0);
        uVar2 = 0xffffffff;
      }
      else {
        param_1[0x1a] = local_60;
        param_1[0x1b] = local_70;
        if (DAT_00462ae8 != 0) {
          __adj_fdiv_m64(0,0x40d58880);
        }
        lVar3 = __ftol();
        param_1[0x19] = (int)lVar3;
        uVar2 = 0;
      }
    }
    else {
      FUN_0043a20d((int)param_1);
      param_1[0x1c] = 0;
      uVar2 = 0xffffffff;
    }
  }
  else {
    MessageBoxA((HWND)*param_1,s_Video_w_out_audio_00461814,(LPCSTR)0x0,0);
    param_1[0x1c] = 0;
    uVar2 = 0xffffffff;
  }
  return uVar2;
}
