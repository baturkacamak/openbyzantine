// rea decompile_all
// name: FUN_00402808
// address: 0x402808
// body: available
// range: 0x402808-0x402a43 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00402808(void)

{
  DWORD DVar1;
  int *piVar2;
  float10 fVar3;
  float fVar4;
  tagRECT local_2c;
  tagRECT local_1c;
  undefined4 local_c;
  undefined4 local_8;
  
  local_c = 0;
  local_8 = 0x3ff00000;
  DVar1 = timeGetTime();
  if (DAT_0045c070 + _DAT_0045c068 < DVar1) {
    if (_DAT_0045c06c == 0.0) {
      _DAT_0045c06c = 0.0001;
      FUN_0040278c();
      DAT_0045c070 = 100;
    }
    else {
      _DAT_0045c06c = _DAT_0045c06c + 0.02;
      if (0.1 < _DAT_0045c06c) {
        _DAT_0045c06c = 0.0;
        DAT_0045c074 = 0x14;
        DAT_0045c070 = 5000;
      }
    }
    _DAT_0045c068 = timeGetTime();
  }
  if (_DAT_0045c06c == 0.0) {
    FUN_00401ee6(0,0,0,0,0);
  }
  else {
    DAT_0045c074 = DAT_0045c074 + 100;
    if (400 < DAT_0045c074) {
      DAT_0045c074 = 0x14;
    }
    FUN_0042427b(DAT_00464880,0,0,0,0,local_c,local_8,SUB84((double)_DAT_0045c06c,0),
                 (int)((ulonglong)(double)_DAT_0045c06c >> 0x20));
    if (DAT_0045c074 != 0x14) {
      piVar2 = (int *)FUN_00418040(DAT_0047a8a4);
      SetRect(&local_1c,0,DAT_0045c074,0x280,0x1a4);
      SetRect(&local_2c,0,0x14,0x280,DAT_0045c074);
      (**(code **)(*piVar2 + 0x1c))(piVar2,0,0x14,DAT_00486888,&local_1c);
      (**(code **)(*piVar2 + 0x1c))
                (piVar2,0,(local_1c.bottom + 0x14) - DAT_0045c074,DAT_00486888,&local_2c,0x10);
      (**(code **)(*DAT_00486888 + 0x1c))(DAT_00486888,0,0x14,piVar2,&DAT_0045c048,0x10);
    }
    piVar2 = DAT_00486888;
    fVar3 = (float10)FUN_00402e00();
    fVar4 = (float)fVar3;
    fVar3 = (float10)FUN_00402e70();
    FUN_004176d8((float)(fVar3 + (float10)180.0),fVar4,piVar2);
    FUN_004181c5(DAT_00486888,&DAT_0045c048,DAT_0045c048,DAT_0045c04c);
  }
  return;
}

