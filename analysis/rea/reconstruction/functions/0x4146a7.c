// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4146a7
// name: FUN_004146a7
// ranges: 0x4146a7-0x4147f9; 0x4147ff-0x414902; 0x414a08-0x414a52
// calls: 0x4146a7

undefined4 __thiscall
FUN_004146a7(void *this,undefined4 *param_1,undefined4 *param_2,undefined4 *param_3)

{
  int iVar1;
  undefined4 uVar2;
  
  iVar1 = (**(code **)(**(int **)((int)this + 8) + 100))
                    (*(undefined4 *)((int)this + 8),0,(int)this + 0xc,1,0);
  if (iVar1 == 0) {
    *param_1 = *(undefined4 *)((int)this + 0x30);
    *param_2 = *(undefined4 *)((int)this + 0x1c);
    *param_3 = *(undefined4 *)((int)this + 0x14);
    uVar2 = 0;
  }
  else {
    if (iVar1 < -0x7fffbffa) {
      if (iVar1 == -0x7fffbffb) {
        OutputDebugStringA(s_Blt_failed_err___DDERR_GENERIC_0045ef2c);
      }
      else if (iVar1 == -0x7fffbfff) {
        OutputDebugStringA(s_Blt_failed_err___DDERR_UNSUPPORT_0045ee98);
      }
    }
    else if (iVar1 < -0x7789ff91) {
      if (iVar1 == -0x7789ff92) {
        OutputDebugStringA(s_Blt_failed_err___DDERR_INVALIDCL_0045eddc);
      }
      else if (iVar1 == -0x7ff8ffa9) {
        OutputDebugStringA(s_Blt_failed_err___DDERR_INVALIDPA_0045ee04);
      }
    }
    else if (iVar1 < -0x7789ff69) {
      if (iVar1 == -0x7789ff6a) {
        OutputDebugStringA(s_Blt_failed_err___DDERR_INVALIDRE_0045ee2c);
      }
      else if (iVar1 == -0x7789ff7e) {
        OutputDebugStringA(s_Blt_failed_err___DDERR_INVALIDOB_0045ef4c);
      }
    }
    else if (iVar1 < -0x7789ff32) {
      if (iVar1 == -0x7789ff33) {
        OutputDebugStringA(s_Blt_failed_err___DDERR_NOCLIPLIS_0045ef98);
      }
      else if (iVar1 == -0x7789ff4c) {
        OutputDebugStringA(s_Blt_failed_err___DDERR_NOALPHAHW_0045ef74);
      }
    }
    else if (iVar1 < -0x7789fee7) {
      if (iVar1 == -0x7789fee8) {
        OutputDebugStringA(s_Blt_failed_err___DDERR_NORASTERO_0045eebc);
      }
      else if (iVar1 == -0x7789ff06) {
        OutputDebugStringA(s_Blt_failed_err___DDERR_NOMIRRORH_0045efe0);
      }
    }
    else if (iVar1 < -0x7789fec9) {
      if (iVar1 == -0x7789feca) {
        OutputDebugStringA(s_Blt_failed_err___DDERR_NOSTRETCH_0045eee4);
      }
      else if (iVar1 == -0x7789fede) {
        OutputDebugStringA(s_Blt_failed_err___DDERR_NOROTATIO_0045f004);
      }
    }
    else {
      switch(iVar1) {
      case -0x7789feac:
        OutputDebugStringA(s_Blt_failed_err___DDERR_NOZBUFFER_0045ef08);
        break;
      case -0x7789fe52:
        OutputDebugStringA(s_Blt_failed_err___DDERR_SURFACEBU_0045f02c);
        break;
      case -0x7789fe3e:
        OutputDebugStringA(s_Blt_failed_err___DDERR_SURFACELO_0045efbc);
        break;
      case -0x7789fdc1:
        OutputDebugStringA(s_Blt_failed_err___DDERR_NOBLTHW_0045ee50);
        break;
      case -0x7789fdc0:
        OutputDebugStringA(s_Blt_failed_err___DDERR_NODDROPSH_0045ee74);
      }
    }
    *param_1 = 0;
    *param_2 = 0;
    uVar2 = 0xffffffff;
  }
  return uVar2;
}
