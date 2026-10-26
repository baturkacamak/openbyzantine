// rea decompile_all
// name: FUN_0042571c
// address: 0x42571c
// body: available
// range: 0x42571c-0x425951 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


bool __cdecl FUN_0042571c(int param_1,int param_2,FILE *param_3,char param_4,byte param_5)

{
  bool bVar1;
  undefined4 local_14;
  undefined1 local_10;
  undefined1 local_c;
  
  bVar1 = false;
  if (param_2 == 0) {
    _fputc(0xff,param_3);
    _fputc((uint)param_5,param_3);
  }
  else if (param_4 == '\0') {
    local_c = 0xff;
    do {
      local_c = local_c + 1;
      local_14 = *(int *)(param_1 + 0x4c4 + (uint)local_c * 4);
      if ((local_14 != 0) && (local_14 != param_2)) {
        local_10 = 0;
        for (; ((local_10 < param_5 && (local_14 != param_2)) && (local_14 != 0));
            local_14 = *(int *)(local_14 + 8)) {
          local_10 = local_10 + 1;
        }
      }
    } while ((local_c < 0x31) && (local_14 != param_2));
    if (local_c != 0x31) {
      _fputc((uint)local_c,param_3);
      _fputc((uint)param_5,param_3);
    }
    else {
      FUN_0040ac43();
      FUN_0040ac38();
      _fputc(0xff,param_3);
      _fputc((uint)param_5,param_3);
    }
    bVar1 = local_c == 0x31;
  }
  else {
    local_c = 0;
    while ((local_c < 0x32 && (*(int *)(param_1 + 0x58c + (uint)local_c * 4) != param_2))) {
      local_c = local_c + 1;
    }
    if (local_c == 0x32) {
      FUN_0040ac43();
      FUN_0040ac38();
      _fputc(0xff,param_3);
      _fputc((uint)param_5,param_3);
      bVar1 = true;
    }
    else {
      _fputc((uint)local_c,param_3);
      _fputc((uint)param_5,param_3);
    }
  }
  return bVar1;
}

