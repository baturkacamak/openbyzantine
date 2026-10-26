// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x413e6b
// name: FUN_00413e6b
// ranges: 0x413e6b-0x41403c; 0x414042-0x414145; 0x41424b-0x414259
// calls: 0x413e6b

int __thiscall FUN_00413e6b(void *this,int param_1,int param_2,int param_3,int param_4,int param_5)

{
  undefined4 local_2c;
  int local_28;
  int local_24;
  int local_20;
  int local_1c;
  tagRECT local_18;
  int local_8;
  
  GetClientRect(*(HWND *)((int)this + 0x98),&local_18);
  ClientToScreen(*(HWND *)((int)this + 0x98),(LPPOINT)&local_18);
  local_28 = param_1;
  local_24 = param_2;
  local_20 = param_3 + param_1;
  local_1c = param_4 + param_2;
  local_18.left = local_18.left + param_1;
  local_18.top = local_18.top + param_2;
  local_18.right = local_18.left + param_3;
  local_18.bottom = local_18.top + param_4;
  if (param_5 == 0) {
    local_2c = 0;
  }
  else {
    local_2c = 0x1000000;
  }
  local_8 = (**(code **)(**(int **)((int)this + 4) + 0x14))
                      (*(undefined4 *)((int)this + 4),&local_18,*(undefined4 *)((int)this + 8),
                       &local_28,local_2c,0);
  if (local_8 < -0x7fffbffa) {
    if (local_8 == -0x7fffbffb) {
      OutputDebugStringA(s_DDERR_GENERIC_0045eb2c);
    }
    else if (local_8 == -0x7fffbfff) {
      OutputDebugStringA(s_DDERR_UNSUPPORTED_0045ec70);
    }
  }
  else if (local_8 < -0x7789ff91) {
    if (local_8 == -0x7789ff92) {
      OutputDebugStringA(s_DDERR_INVALIDCLIPLIST_0045eb3c);
    }
    else if (local_8 == -0x7ff8ffa9) {
      OutputDebugStringA(s_DDERR_INVALIDPARAMS_0045eb6c);
    }
  }
  else if (local_8 < -0x7789ff69) {
    if (local_8 == -0x7789ff6a) {
      OutputDebugStringA(s_DDERR_INVALIDRECT_0045eb84);
    }
    else if (local_8 == -0x7789ff7e) {
      OutputDebugStringA(s_DDERR_INVALIDOBJECT_0045eb54);
    }
  }
  else if (local_8 < -0x7789ff32) {
    if (local_8 == -0x7789ff33) {
      OutputDebugStringA(s_DDERR_NOCLIPLIST_0045ebbc);
    }
    else if (local_8 == -0x7789ff4c) {
      OutputDebugStringA(s_DDERR_NOALPHAHW_0045eb98);
    }
  }
  else if (local_8 < -0x7789fee7) {
    if (local_8 == -0x7789fee8) {
      OutputDebugStringA(s_DDERR_NORASTEROPHW_0045ebf8);
    }
    else if (local_8 == -0x7789ff06) {
      OutputDebugStringA(s_DDERR_NOMIRRORHW_0045ebe4);
    }
  }
  else if (local_8 < -0x7789fec9) {
    if (local_8 == -0x7789feca) {
      OutputDebugStringA(s_DDERR_NOSTRETCHHW_0045ec20);
    }
    else if (local_8 == -0x7789fede) {
      OutputDebugStringA(s_DDERR_NOROTATIONHW_0045ec0c);
    }
  }
  else {
    switch(local_8) {
    case -0x7789feac:
      OutputDebugStringA(s_DDERR_NOZBUFFERHW_0045ec34);
      break;
    case -0x7789fe52:
      OutputDebugStringA(s_DDERR_SURFACEBUSY_0045ec48);
      break;
    case -0x7789fe3e:
      OutputDebugStringA(s_DDERR_SURFACELOST_0045ec5c);
      break;
    case -0x7789fdc1:
      OutputDebugStringA(s_DDERR_NOBLTHW_0045ebac);
      break;
    case -0x7789fdc0:
      OutputDebugStringA(s_DDERR_NODDROPSHW_0045ebd0);
    }
  }
  return local_8;
}
