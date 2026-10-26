// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x422f37
// name: FUN_00422f37
// ranges: 0x422f37-0x42313e
// calls: 0x404d10, 0x404f64, 0x40513f, 0x422f37, 0x42313f, 0x427d1a

undefined4 __cdecl FUN_00422f37(LPSTR param_1,int *param_2)

{
  HWND pHVar1;
  undefined4 uVar2;
  char *pcVar3;
  LPCSTR pCVar4;
  UINT UVar5;
  int local_24;
  undefined4 local_20;
  undefined4 local_1c;
  uint local_18;
  short *local_10;
  int local_c;
  MMRESULT local_8;
  
  if (param_2 == (int *)0x0) {
    local_24 = 0;
  }
  else {
    local_24 = param_2[5];
  }
  local_8 = FUN_00404d10(param_1,&DAT_004867a4,(int *)&DAT_004867a0,(LPMMCKINFO)&DAT_004867bc);
  if (local_8 == 0) {
    if (*DAT_004867a0 == 1) {
      local_8 = FUN_00404f64(&DAT_004867a4,(LPMMCKINFO)&DAT_004867a8,(MMCKINFO *)&DAT_004867bc);
      if (local_8 == 0) {
        DAT_004867d4 = (uint)(*(int *)(DAT_004867a0 + 4) * 0x78) / 100;
        _memset(&local_20,0,0x14);
        local_20 = 0x14;
        local_1c = 0x100e0;
        local_18 = DAT_004867d4;
        local_10 = DAT_004867a0;
        local_c = (**(code **)(*DAT_00461934 + 0xc))(DAT_00461934,&local_20,&DAT_0045fd14,0);
        if (local_c == 0) {
          DAT_004867d0 = DAT_0045fd14;
          DAT_004867e8 = local_24;
          FUN_0042313f(DAT_0045fd2c);
          DAT_0045fd2c = 0;
          if (param_2 != (int *)0x0) {
            FUN_00427d1a(param_2);
          }
          uVar2 = 0;
        }
        else {
          UVar5 = 0x30;
          pCVar4 = (LPCSTR)0x0;
          pcVar3 = s_Create_sound_buffer_failed_0045fd90;
          pHVar1 = GetActiveWindow();
          MessageBoxA(pHVar1,pcVar3,pCVar4,UVar5);
          uVar2 = 0xffffff9a;
        }
      }
      else {
        UVar5 = 0x30;
        pCVar4 = (LPCSTR)0x0;
        pcVar3 = s_Wave_seek_failed_0045fd7c;
        pHVar1 = GetActiveWindow();
        MessageBoxA(pHVar1,pcVar3,pCVar4,UVar5);
        FUN_0040513f(&DAT_004867a4,(int *)&DAT_004867a0);
        uVar2 = 0xffffff99;
      }
    }
    else {
      UVar5 = 0x30;
      pCVar4 = (LPCSTR)0x0;
      pcVar3 = s_Wave_file_not_PCM_format_0045fd60;
      pHVar1 = GetActiveWindow();
      MessageBoxA(pHVar1,pcVar3,pCVar4,UVar5);
      FUN_0040513f(&DAT_004867a4,(int *)&DAT_004867a0);
      uVar2 = 0xffffff9b;
    }
  }
  else {
    UVar5 = 0x30;
    pCVar4 = (LPCSTR)0x0;
    pcVar3 = s_Couldn_t_open_wave_file_0045fd48;
    pHVar1 = GetActiveWindow();
    MessageBoxA(pHVar1,pcVar3,pCVar4,UVar5);
    uVar2 = 0xffffff9c;
  }
  return uVar2;
}
