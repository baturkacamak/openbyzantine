// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x41425a
// name: FUN_0041425a
// ranges: 0x41425a-0x414407; 0x41440d-0x414510; 0x414616-0x414624
// calls: 0x41425a

int __thiscall FUN_0041425a(void *this,int param_1)

{
  undefined4 local_2c;
  undefined4 local_28;
  undefined4 local_24;
  undefined4 local_20;
  undefined4 local_1c;
  undefined4 local_18;
  undefined4 local_14;
  undefined4 local_10;
  undefined4 local_c;
  int local_8;
  
  local_28 = 0;
  local_24 = 0;
  local_20 = *(undefined4 *)((int)this + 0x18);
  local_1c = *(undefined4 *)((int)this + 0x14);
  local_18 = 0;
  local_14 = 0;
  local_10 = *(undefined4 *)((int)this + 0x18);
  local_c = *(undefined4 *)((int)this + 0x14);
  if (param_1 == 0) {
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
      OutputDebugStringA(s_DDERR_GENERIC_0045ec84);
    }
    else if (local_8 == -0x7fffbfff) {
      OutputDebugStringA(s_DDERR_UNSUPPORTED_0045edc8);
    }
  }
  else if (local_8 < -0x7789ff91) {
    if (local_8 == -0x7789ff92) {
      OutputDebugStringA(s_DDERR_INVALIDCLIPLIST_0045ec94);
    }
    else if (local_8 == -0x7ff8ffa9) {
      OutputDebugStringA(s_DDERR_INVALIDPARAMS_0045ecc4);
    }
  }
  else if (local_8 < -0x7789ff69) {
    if (local_8 == -0x7789ff6a) {
      OutputDebugStringA(s_DDERR_INVALIDRECT_0045ecdc);
    }
    else if (local_8 == -0x7789ff7e) {
      OutputDebugStringA(s_DDERR_INVALIDOBJECT_0045ecac);
    }
  }
  else if (local_8 < -0x7789ff32) {
    if (local_8 == -0x7789ff33) {
      OutputDebugStringA(s_DDERR_NOCLIPLIST_0045ed14);
    }
    else if (local_8 == -0x7789ff4c) {
      OutputDebugStringA(s_DDERR_NOALPHAHW_0045ecf0);
    }
  }
  else if (local_8 < -0x7789fee7) {
    if (local_8 == -0x7789fee8) {
      OutputDebugStringA(s_DDERR_NORASTEROPHW_0045ed50);
    }
    else if (local_8 == -0x7789ff06) {
      OutputDebugStringA(s_DDERR_NOMIRRORHW_0045ed3c);
    }
  }
  else if (local_8 < -0x7789fec9) {
    if (local_8 == -0x7789feca) {
      OutputDebugStringA(s_DDERR_NOSTRETCHHW_0045ed78);
    }
    else if (local_8 == -0x7789fede) {
      OutputDebugStringA(s_DDERR_NOROTATIONHW_0045ed64);
    }
  }
  else {
    switch(local_8) {
    case -0x7789feac:
      OutputDebugStringA(s_DDERR_NOZBUFFERHW_0045ed8c);
      break;
    case -0x7789fe52:
      OutputDebugStringA(s_DDERR_SURFACEBUSY_0045eda0);
      break;
    case -0x7789fe3e:
      OutputDebugStringA(s_DDERR_SURFACELOST_0045edb4);
      break;
    case -0x7789fdc1:
      OutputDebugStringA(s_DDERR_NOBLTHW_0045ed04);
      break;
    case -0x7789fdc0:
      OutputDebugStringA(s_DDERR_NODDROPSHW_0045ed28);
    }
  }
  return local_8;
}
