// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x43cda0
// name: FUN_0043cda0
// ranges: 0x43cda0-0x43d4c8
// calls: 0x404f64, 0x404fe3, 0x4161f6, 0x43cda0

void FUN_0043cda0(void)

{
  LONG LVar1;
  int iVar2;
  uint local_40;
  uint local_3c;
  size_t local_38;
  size_t local_34;
  uint local_30;
  undefined1 local_2c [4];
  MMRESULT local_28;
  uint local_24;
  int local_20;
  int local_1c;
  void *local_18;
  byte local_14 [4];
  void *local_10;
  void *local_c;
  uint local_8;
  
  local_20 = 0;
  LVar1 = InterlockedExchange((LONG *)&DAT_004618b0,1);
  if (LVar1 == 0) {
    DAT_0046e0d8 = (HWND)FUN_004161f6();
    (**(code **)(*DAT_004867d0 + 0x24))(DAT_004867d0,local_14);
    if ((local_14[0] & 2) != 0) {
      local_1c = (**(code **)(*DAT_004867d0 + 0x50))(DAT_004867d0);
      if (local_1c < 0) {
        InterlockedExchange((LONG *)&DAT_004618b0,0);
        return;
      }
      DAT_004867d8 = 0;
      local_20 = 1;
    }
    (**(code **)(*DAT_004867d0 + 0x10))(DAT_004867d0,&local_8,local_2c);
    if ((DAT_004867d8 == local_8) && (local_20 == 0)) {
      InterlockedExchange((LONG *)&DAT_004618b0,0);
    }
    else if ((DAT_004867ec == 0) || (DAT_004867e0 != 0)) {
      if (local_8 < DAT_004867d8) {
        local_30 = (DAT_004867d4 + local_8) - DAT_004867d8;
      }
      else {
        local_30 = local_8 - DAT_004867d8;
      }
      if (local_20 == 0) {
        local_40 = local_30;
      }
      else {
        local_40 = (DAT_004867d4 * 2) / 6;
      }
      DAT_004867dc = DAT_004867dc + local_30;
      if ((DAT_004867ec == 0) || (DAT_004867e0 == 0)) {
        iVar2 = (**(code **)(*DAT_004867d0 + 0x2c))
                          (DAT_004867d0,DAT_004867d8,local_40,&local_10,&local_34,&local_18,
                           &local_38,0);
        if (iVar2 == 0) {
          if ((local_34 == 0) || (DAT_004867e4 != 0)) {
            if ((local_34 != 0) && (DAT_004867e4 != 0)) {
              _memset(local_10,-(uint)(*(short *)(DAT_004867a0 + 0xe) == 8) & 0x80,local_34);
            }
          }
          else {
            local_28 = FUN_00404fe3(DAT_004867a4,local_34,(int)local_10,0x4867a8,&local_24);
            if (local_24 < local_34) {
              if (DAT_004867e8 == 0) {
                if (*(short *)(DAT_004867a0 + 0xe) == 8) {
                  _memset((void *)(local_24 + (int)local_10),0x80,local_34 - local_24);
                }
                else if (*(short *)(DAT_004867a0 + 0xe) == 0x10) {
                  _memset((void *)(local_24 + (int)local_10),0,local_34 - local_24);
                }
                DAT_004867ec = 1;
                if (DAT_004867d8 < local_8) {
                  DAT_004867e0 = (DAT_004867d8 + DAT_004867d4) - local_8;
                }
                else {
                  DAT_004867e0 = DAT_004867d8 - local_8;
                }
              }
              else {
                local_c = local_10;
                local_3c = local_34;
                do {
                  local_c = (void *)((int)local_c + local_24);
                  local_3c = local_3c - local_24;
                  local_28 = FUN_00404f64(&DAT_004867a4,(LPMMCKINFO)&DAT_004867a8,
                                          (MMCKINFO *)&DAT_004867bc);
                  local_28 = FUN_00404fe3(DAT_004867a4,local_3c,(int)local_c,0x4867a8,&local_24);
                } while (local_24 < local_3c);
              }
            }
          }
          if ((local_38 == 0) || (DAT_004867e4 != 0)) {
            if ((local_18 != (void *)0x0) && ((local_38 != 0 && (DAT_004867e4 != 0)))) {
              _memset(local_18,-(uint)(*(short *)(DAT_004867a0 + 0xe) == 8) & 0x80,local_38);
            }
          }
          else {
            local_28 = FUN_00404fe3(DAT_004867a4,local_38,(int)local_18,0x4867a8,&local_24);
            if (local_24 < local_38) {
              if (DAT_004867e8 == 0) {
                if (*(short *)(DAT_004867a0 + 0xe) == 8) {
                  _memset((void *)(local_24 + (int)local_18),0x80,local_38 - local_24);
                }
                else if (*(short *)(DAT_004867a0 + 0xe) == 0x10) {
                  _memset((void *)(local_24 + (int)local_18),0,local_38 - local_24);
                }
                DAT_004867ec = 1;
                if (DAT_004867d8 < local_8) {
                  DAT_004867e0 = (DAT_004867d8 + DAT_004867d4) - local_8;
                }
                else {
                  DAT_004867e0 = DAT_004867d8 - local_8;
                }
              }
              else {
                local_c = local_18;
                local_3c = local_38;
                do {
                  local_c = (void *)((int)local_c + local_24);
                  local_3c = local_3c - local_24;
                  local_28 = FUN_00404f64(&DAT_004867a4,(LPMMCKINFO)&DAT_004867a8,
                                          (MMCKINFO *)&DAT_004867bc);
                  local_28 = FUN_00404fe3(DAT_004867a4,local_3c,(int)local_c,0x4867a8,&local_24);
                } while (local_24 < local_3c);
              }
            }
          }
          (**(code **)(*DAT_004867d0 + 0x4c))(DAT_004867d0,local_10,local_34,local_18,local_38);
          DAT_004867d8 = DAT_004867d8 + local_40;
          if (DAT_004867d4 <= DAT_004867d8) {
            DAT_004867d8 = DAT_004867d8 - DAT_004867d4;
          }
          if (local_20 != 0) {
            (**(code **)(*DAT_004867d0 + 0x30))(DAT_004867d0,0,0,1);
          }
          InterlockedExchange((LONG *)&DAT_004618b0,0);
        }
        else {
          OutputDebugStringA(s_TimeFunc___could_not_lock_Direct_004618e0);
          InterlockedExchange((LONG *)&DAT_004618b0,0);
        }
      }
      else {
        if (DAT_004867e0 < local_30) {
          DAT_004867e0 = 0;
        }
        else {
          DAT_004867e0 = DAT_004867e0 - local_30;
        }
        iVar2 = (**(code **)(*DAT_004867d0 + 0x2c))
                          (DAT_004867d0,DAT_004867d8,local_40,&local_10,&local_34,&local_18,
                           &local_38,0);
        if (iVar2 == 0) {
          _memset(local_10,-(uint)(*(short *)(DAT_004867a0 + 0xe) == 8) & 0x80,local_34);
          if ((local_18 != (void *)0x0) && (local_38 != 0)) {
            _memset(local_18,-(uint)(*(short *)(DAT_004867a0 + 0xe) == 8) & 0x80,local_38);
          }
          (**(code **)(*DAT_004867d0 + 0x4c))(DAT_004867d0,local_10,local_34,local_18,local_38);
          DAT_004867d8 = DAT_004867d8 + local_40;
          if (DAT_004867d4 <= DAT_004867d8) {
            DAT_004867d8 = DAT_004867d8 - DAT_004867d4;
          }
          InterlockedExchange((LONG *)&DAT_004618b0,0);
        }
        else {
          OutputDebugStringA(s_TimeFunc___could_not_lock_Direct_004618b4);
          InterlockedExchange((LONG *)&DAT_004618b0,0);
        }
      }
    }
    else {
      if (DAT_004867e4 == 0) {
        DAT_004867e4 = 1;
        PostMessageA(DAT_0046e0d8,0x500,0,0);
      }
      InterlockedExchange((LONG *)&DAT_004618b0,0);
    }
  }
  return;
}
