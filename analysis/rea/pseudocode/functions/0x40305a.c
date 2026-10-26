// rea decompile_all
// name: FUN_0040305a
// address: 0x40305a
// body: available
// range: 0x40305a-0x4036cd inclusive
// range: 0x4036d3-0x403ae1 inclusive
// range: 0x403c1f-0x403c23 inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_0040305a(int param_1,char *param_2)

{
  if (param_1 < -0x7fffbffa) {
    if (param_1 == -0x7fffbffb) {
      _sprintf(param_2,s_DDERR_GENERIC_0045c284);
      return;
    }
    if (param_1 == -0x7fffbfff) {
      _sprintf(param_2,s_DDERR_UNSUPPORTED_0045c5a4);
      return;
    }
  }
  else if (param_1 < -0x7ff8ffa8) {
    if (param_1 == -0x7ff8ffa9) {
      _sprintf(param_2,s_DDERR_INVALIDPARAMS_0045c310);
      return;
    }
    if (param_1 == -0x7ff8fff2) {
      _sprintf(param_2,s_DDERR_OUTOFMEMORY_0045c4e0);
      return;
    }
  }
  else if (param_1 < -0x7789ffa0) {
    if (param_1 == -0x7789ffa1) {
      _sprintf(param_2,s_DDERR_INCOMPATIBLEPRIMARY_0045c2b0);
      return;
    }
    if (param_1 == -0x7789ffc9) {
      _sprintf(param_2,s_DDERR_EXCEPTION_0045c270);
      return;
    }
  }
  else if (param_1 < -0x7789ff91) {
    if (param_1 == -0x7789ff92) {
      _sprintf(param_2,s_DDERR_INVALIDCLIPLIST_0045c2e0);
      return;
    }
    if (param_1 == -0x7789ff9c) {
      _sprintf(param_2,s_DDERR_INVALIDCAPS_0045c2cc);
      return;
    }
  }
  else if (param_1 < -0x7789ff6e) {
    if (param_1 == -0x7789ff6f) {
      _sprintf(param_2,s_DDERR_INVALIDPIXELFORMAT_0045c328);
      return;
    }
    if (param_1 == -0x7789ff7e) {
      _sprintf(param_2,s_DDERR_INVALIDOBJECT_0045c2f8);
      return;
    }
  }
  else if (param_1 < -0x7789ff5f) {
    if (param_1 == -0x7789ff60) {
      _sprintf(param_2,s_DDERR_LOCKEDSURFACES_0045c678);
      return;
    }
    if (param_1 == -0x7789ff6a) {
      _sprintf(param_2,s_DDERR_INVALIDRECT_0045c344);
      return;
    }
  }
  else if (param_1 < -0x7789ff32) {
    if (param_1 == -0x7789ff33) {
      _sprintf(param_2,s_DDERR_NOCLIPLIST_0045c37c);
      return;
    }
    if (param_1 == -0x7789ff4c) {
      _sprintf(param_2,s_DDERR_NOALPHAHW_0045c358);
      return;
    }
  }
  else if (param_1 < -0x7789ff1e) {
    if (param_1 == -0x7789ff1f) {
      _sprintf(param_2,s_DDERR_NOEXCLUSIVEMODE_0045c3f0);
      return;
    }
    if (param_1 == -0x7789ff2c) {
      _sprintf(param_2,s_DDERR_NOCOOPERATIVELEVELSET_0045c390);
      return;
    }
  }
  else if (param_1 < -0x7789ff05) {
    if (param_1 == -0x7789ff06) {
      _sprintf(param_2,s_DDERR_NOMIRRORHW_0045c41c);
      return;
    }
    if (param_1 == -0x7789ff1a) {
      _sprintf(param_2,s_DDERR_NOFLIPHW_0045c408);
      return;
    }
  }
  else if (param_1 < -0x7789fee7) {
    if (param_1 == -0x7789fee8) {
      _sprintf(param_2,s_DDERR_NORASTEROPHW_0045c460);
      return;
    }
    if (param_1 == -0x7789fefc) {
      _sprintf(param_2,s_DDERR_NOOVERLAYHW_0045c4b8);
      return;
    }
  }
  else if (param_1 < -0x7789fec9) {
    if (param_1 == -0x7789feca) {
      _sprintf(param_2,s_DDERR_NOSTRETCHHW_0045c490);
      return;
    }
    if (param_1 == -0x7789fede) {
      _sprintf(param_2,s_DDERR_NOROTATIONHW_0045c478);
      return;
    }
  }
  else if (param_1 < -0x7789fe83) {
    if (param_1 == -0x7789fe84) {
      _sprintf(param_2,s_DDERR_OUTOFVIDEOMEMORY_0045c4f4);
      return;
    }
    if (param_1 == -0x7789feac) {
      _sprintf(param_2,s_DDERR_NOZBUFFERHW_0045c4cc);
      return;
    }
  }
  else if (param_1 < -0x7789fe51) {
    if (param_1 == -0x7789fe52) {
      _sprintf(param_2,s_DDERR_SURFACEBUSY_0045c548);
      return;
    }
    if (param_1 == -0x7789fe7d) {
      _sprintf(param_2,s_DDERR_PALETTEBUSY_0045c510);
      return;
    }
  }
  else if (param_1 < -0x7789fe47) {
    if (param_1 == -0x7789fe48) {
      _sprintf(param_2,s_DDERR_SURFACEISOBSCURED_0045c55c);
      return;
    }
    if (param_1 == -0x7789fe4d) {
      _sprintf(param_2,s_DDERR_CANTLOCKSURFACE_0045c690);
      return;
    }
  }
  else if (param_1 < -0x7789fe33) {
    if (param_1 == -0x7789fe34) {
      _sprintf(param_2,s_SURFACENOTATTACHED_0045c58c);
      return;
    }
    if (param_1 == -0x7789fe3e) {
      _sprintf(param_2,s_DDERR_SURFACELOST_0045c578);
      return;
    }
  }
  else if (param_1 < -0x7789fdf7) {
    if (param_1 == -0x7789fdf8) {
      _sprintf(param_2,s_DDERR_UNSUPPORTEDMASK_0045c5d4);
      return;
    }
    if (param_1 == -0x7789fe02) {
      _sprintf(param_2,s_DDERR_UNSUPPORTEDFORMAT_0045c5b8);
      return;
    }
  }
  else if (param_1 < -0x7789fde3) {
    if (param_1 == -0x7789fde4) {
      _sprintf(param_2,s_DDERR_WASSTILLDRAWING_0045c60c);
      return;
    }
    if (param_1 == -0x7789fde7) {
      _sprintf(param_2,s_DDERR_VERTICALBLANKINPROGRESS_0045c5ec);
      return;
    }
  }
  else if (param_1 < -0x7789fdcc) {
    if (param_1 == -0x7789fdcd) {
      _sprintf(param_2,s_DDERR_NODIRECTDRAWHW_0045c3c4);
      return;
    }
    if (param_1 == -0x7789fdd0) {
      _sprintf(param_2,s_DDERR_XALIGN_0045c638);
      return;
    }
  }
  else if (param_1 < -0x7789fdc3) {
    if (param_1 == -0x7789fdc4) {
      _sprintf(param_2,s_DDERR_NOPALETTEATTACHED_0045c430);
      return;
    }
    if (param_1 == -0x7789fdcc) {
      _sprintf(param_2,s_DDERR_PRIMARYSURFACEALREADYEXIST_0045c524);
      return;
    }
    if (param_1 == -0x7789fdcb) {
      _sprintf(param_2,s_DDERR_NOEMULATION_0045c3dc);
      return;
    }
  }
  else if (param_1 < -0x7789fdbb) {
    if (param_1 == -0x7789fdbc) {
      _sprintf(param_2,s_DDERR_NOTAOVERLAY_0045c4a4);
      return;
    }
    if (param_1 == -0x7789fdc3) {
      _sprintf(param_2,s_DDERR_NOPALETTEHW_0045c44c);
      return;
    }
    if (param_1 == -0x7789fdc1) {
      _sprintf(param_2,s_DDERR_NOBLTHW_0045c36c);
      return;
    }
    if (param_1 == -0x7789fdc0) {
      _sprintf(param_2,s_DDERR_NODDROPSHW_0045c3b0);
      return;
    }
  }
  else if (param_1 < -0x7789fdb4) {
    if (param_1 == -0x7789fdb5) {
      _sprintf(param_2,s_DDERR_WRONGMODE_0045c624);
      return;
    }
    if (param_1 == -0x7789fdb8) {
      _sprintf(param_2,s_DDERR_NOTLOCKED_0045c6a8);
      return;
    }
  }
  else if (param_1 < -0x7789fdb0) {
    if (param_1 == -0x7789fdb1) {
      _sprintf(param_2,s_DDERR_NOMIPMAPHW_0045c648);
      return;
    }
    if (param_1 == -0x7789fdb4) {
      _sprintf(param_2,s_DDERR_IMPLICITLYCREATED_0045c294);
      return;
    }
  }
  else if (param_1 < -0x7789fd7f) {
    if (param_1 == -0x7789fd80) {
      _sprintf(param_2,s_DDERR_CANTPAGELOCK_0045c6bc);
      return;
    }
    if (param_1 == -0x7789fdb0) {
      _sprintf(param_2,s_DDERR_INVALIDSURFACETYPE_0045c65c);
      return;
    }
  }
  else if (param_1 < -0x7789fd57) {
    if (param_1 == -0x7789fd58) {
      _sprintf(param_2,s_DDERR_CANTPAGELOCK_0045c6ec);
      return;
    }
    if (param_1 == -0x7789fd6c) {
      _sprintf(param_2,s_DDERR_CANTPAGELOCK_0045c6d4);
      return;
    }
  }
  else if (param_1 < -0x7788fff5) {
    if (param_1 == -0x7788fff6) {
      _sprintf(param_2,s_DPERR_ACCESSDENIED_0045c704);
      return;
    }
    if (param_1 == -0x7788fffb) {
      _sprintf(param_2,s_DPERR_ALREADYINITIALIZED_0045c734);
      return;
    }
  }
  else if (param_1 < -0x7788ffe1) {
    if (param_1 == -0x7788ffe2) {
      _sprintf(param_2,s_DPERR_BUFFERTOOSMALL_0045c750);
      return;
    }
    if (param_1 == -0x7788ffec) {
      _sprintf(param_2,s_DPERR_ACTIVEPLAYERS_0045c71c);
      return;
    }
  }
  else {
    switch(param_1) {
    case -0x7788ffd8:
      _sprintf(param_2,s_DPERR_CANTADDPLAYER_0045c778);
      return;
    case -0x7788ffc4:
      _sprintf(param_2,s_DPERR_CANTCREATEPLAYER_0045c790);
      return;
    case -0x7788ffb0:
      _sprintf(param_2,s_DPERR_CAPSNOTAVAILABLEYET_0045c7ac);
      return;
    case -0x7788ffa6:
      _sprintf(param_2,s_DPERR_EXCEPTION_0045c7c8);
      return;
    case -0x7788ff88:
      _sprintf(param_2,s_DPERR_INVALIDFLAGS_0045c7dc);
      return;
    case -0x7788ff7e:
      _sprintf(param_2,s_DPERR_INVALIDOBJECT_0045c7f4);
      return;
    case -0x7788ff6a:
      _sprintf(param_2,s_DPERR_INVALIDPLAYER_0045c80c);
      return;
    case -0x7788ff60:
      _sprintf(param_2,s_DPERR_NOCAPS_0045c824);
      return;
    case -0x7788ff56:
      _sprintf(param_2,s_DPERR_NOCONNECTION_0045c834);
      return;
    case -0x7788ff42:
      _sprintf(param_2,s_DPERR_NOMESSAGES_0045c84c);
      return;
    case -0x7788ff38:
      _sprintf(param_2,s_DPERR_NONAMESERVERFOUND_0045c860);
      return;
    case -0x7788ff2e:
      _sprintf(param_2,s_DPERR_NOPLAYERS_0045c87c);
      return;
    case -0x7788ff24:
      _sprintf(param_2,s_DPERR_NOSESSIONS_0045c890);
      return;
    case -0x7788ff1a:
      _sprintf(param_2,s_DPERR_SENDTOOBIG_0045c8a4);
      return;
    case -0x7788ff10:
      _sprintf(param_2,s_DPERR_TIMEOUT_0045c8b8);
      return;
    case -0x7788ff06:
      _sprintf(param_2,s_DPERR_UNAVAILABLE_0045c8c8);
      return;
    case -0x7788fef2:
      _sprintf(param_2,s_DPERR_BUSY_0045c768);
      return;
    case -0x7788fee8:
      _sprintf(param_2,s_DPERR_USERCANCEL_0045c8dc);
      return;
    }
  }
  _sprintf(param_2,s_no_error_associated___x_0045c8f0,param_1);
  return;
}

