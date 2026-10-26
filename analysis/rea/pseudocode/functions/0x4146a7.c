// rea decompile_all
// name: FUN_004146a7
// address: 0x4146a7
// body: available
// range: 0x4146a7-0x4147f9 inclusive
// range: 0x4147ff-0x414902 inclusive
// range: 0x414a08-0x414a52 inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


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

