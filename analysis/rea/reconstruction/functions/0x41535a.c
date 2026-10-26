// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x41535a
// name: FUN_0041535a
// ranges: 0x41535a-0x4155f8; 0x4155fe-0x415626; 0x415692-0x41590b
// calls: 0x40bf90, 0x40c7b4, 0x414b90, 0x4152dd, 0x41535a, 0x41590c, 0x415a4e, 0x41d45a, 0x41f21e, 0x42c8a0, 0x44ba00

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

uint FUN_0041535a(HINSTANCE param_1,undefined4 param_2,char *param_3)

{
  char *pcVar1;
  bool bVar2;
  int iVar3;
  undefined3 extraout_var;
  BOOL BVar4;
  uint local_cc;
  uint local_c8;
  uint local_c4;
  uint local_c0;
  uint local_b8;
  uint local_b0 [20];
  tagMSG local_60;
  _MEMORYSTATUS local_44;
  int local_24;
  tagMSG *local_20;
  uint local_1c [4];
  uint local_c;
  int local_8;
  
  local_8 = 0;
  local_24 = 0;
  _DAT_00486a00 = param_1;
  do {
    if (*param_3 != '-') {
      if (((DAT_0045f080 == 0) || (SystemParametersInfoA(0x10,0,&DAT_0045f084,0), DAT_0045f084 == 0)
          ) || (local_c = SystemParametersInfoA(0x11,0,(PVOID)0x0,0), local_c != 0)) {
        if (local_8 == 0) {
          FUN_0044ba00(local_1c,(uint *)&DAT_0045f0c0);
        }
        iVar3 = FUN_0041590c(param_1);
        if (iVar3 == 0) {
          local_c = 0;
        }
        else {
          FUN_00414b90();
          bVar2 = FUN_0041f21e();
          if (CONCAT31(extraout_var,bVar2) == 0) {
            local_24 = 1;
          }
          iVar3 = FUN_0040c7b4(local_24);
          if (iVar3 == 0) {
            iVar3 = FUN_00415a4e(0x280,0x1e0,0x10);
            if (iVar3 == 0) {
              FUN_004152dd();
              local_c = 0;
            }
            else {
              while ((DAT_0045f07c == 0 ||
                     (BVar4 = PeekMessageA(&local_60,(HWND)0x0,0,0,0), BVar4 != 0))) {
                BVar4 = GetMessageA(&local_60,DAT_0046c4e8,0,0);
                if (BVar4 == 0) {
                  return local_60.wParam;
                }
                DAT_0046c514 = &local_60;
                local_20 = DAT_0046c514;
                TranslateMessage(&local_60);
                DispatchMessageA(&local_60);
              }
              _memset(&local_44,0,0x20);
              local_44.dwLength = 0x20;
              GlobalMemoryStatus(&local_44);
              if ((double)local_44.dwTotalPhys < 18000000.0) {
                DAT_0045f068 = 1;
              }
              local_c = FUN_0042c8a0(s__b_t_n_s_0045f0c8,local_1c);
              if (local_c == 0xfffc) {
                FUN_0041d45a(local_b0,0x51);
                MessageBoxA(DAT_0046c4e8,(LPCSTR)local_b0,(LPCSTR)0x0,0);
              }
              FUN_004152dd();
              if (DAT_0045f084 != 0) {
                SystemParametersInfoA(0x11,1,(PVOID)0x0,0);
              }
            }
          }
          else {
            local_c = 0;
          }
        }
      }
      else {
        local_c = 0;
      }
      return local_c;
    }
    pcVar1 = param_3 + 2;
    switch(param_3[1]) {
    case 'A':
    case 'a':
      DAT_0045e7a8 = 0;
      break;
    case 'C':
    case 'c':
      if (DAT_00462eec < 2) {
        local_c4 = *(ushort *)(PTR_DAT_00462ce0 + *pcVar1 * 2) & 8;
      }
      else {
        local_c4 = __isctype((int)*pcVar1,8);
      }
      if (local_c4 != 0) {
        pcVar1 = param_3 + 3;
      }
      while( true ) {
        param_3 = pcVar1;
        if (DAT_00462eec < 2) {
          local_c8 = *(ushort *)(PTR_DAT_00462ce0 + *param_3 * 2) & 0x103;
        }
        else {
          local_c8 = __isctype((int)*param_3,0x103);
        }
        if (local_c8 == 0) break;
        if (DAT_00462eec < 2) {
          local_cc = *(ushort *)(PTR_DAT_00462ce0 + *param_3 * 2) & 8;
        }
        else {
          local_cc = __isctype((int)*param_3,8);
        }
        if (local_cc != 0) break;
        *(char *)((int)local_1c + local_8) = *param_3;
        local_8 = local_8 + 1;
        pcVar1 = param_3 + 1;
      }
      *(undefined1 *)((int)local_1c + local_8) = 0;
      pcVar1 = param_3;
      break;
    case 'D':
    case 'd':
      while( true ) {
        param_3 = pcVar1;
        if (DAT_00462eec < 2) {
          local_b8 = *(ushort *)(PTR_DAT_00462ce0 + *param_3 * 2) & 8;
        }
        else {
          local_b8 = __isctype((int)*param_3,8);
        }
        if (local_b8 == 0) break;
        pcVar1 = param_3 + 1;
      }
      pcVar1 = param_3 + 1;
      iVar3 = FUN_0040bf90(*param_3);
      if (iVar3 != 0) {
        return 0;
      }
      if (DAT_00462eec < 2) {
        local_c0 = *(ushort *)(PTR_DAT_00462ce0 + *pcVar1 * 2) & 4;
      }
      else {
        local_c0 = __isctype((int)*pcVar1,4);
      }
      if (local_c0 != 0) {
        local_24 = *pcVar1 + -0x30;
        pcVar1 = param_3 + 2;
      }
      break;
    case 'F':
    case 'f':
      DAT_0045f080 = 1;
      break;
    case 'P':
    case 'p':
      DAT_0045f074 = 1;
      break;
    case 'W':
    case 'w':
      DAT_0045f080 = 0;
      DAT_0045f074 = 0;
    }
    while ((((param_3 = pcVar1, *param_3 == ' ' || (*param_3 == '\r')) || (*param_3 == '\n')) ||
           ((*param_3 == '\t' || (*param_3 == 'x'))))) {
      pcVar1 = param_3 + 1;
    }
  } while( true );
}
