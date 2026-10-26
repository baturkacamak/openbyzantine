// rea decompile_all
// name: FUN_0041cf5f
// address: 0x41cf5f
// body: available
// range: 0x41cf5f-0x41d234 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


int __cdecl FUN_0041cf5f(uint *param_1)

{
  uint unaff_EDI;
  uint local_218 [65];
  FILE *local_114;
  uint local_110;
  uint local_10c [65];
  int local_8;
  
  local_8 = 0;
  DAT_00483624 = (char *)0x0;
  DAT_00483604 = (char *)0x0;
  DAT_0048242c = 0;
  DAT_004859d0 = -1;
  if (DAT_00485c6c == '\0') {
    FUN_0044ba00(local_10c,param_1);
    FUN_0044ba08(local_10c,(uint *)&DAT_0045f364);
    FUN_0044ba08(local_10c,param_1);
    FUN_0040c624(local_218,(uint *)s_PHONEMES_0045f370,local_10c,(uint *)&DAT_0045f368);
    local_114 = (FILE *)FUN_0044bc20((char *)local_218,&DAT_0045f37c);
    if (local_114 == (FILE *)0x0) {
      local_8 = 1;
      local_114 = (FILE *)0x0;
    }
    else {
      local_110 = FUN_0041199e(local_114);
      DAT_00483604 = (char *)FUN_0044c2f0(local_110 << 2);
      __fread_lk(DAT_00483604,local_110,4,local_114);
      _fclose(local_114);
      FUN_0040c624(local_218,(uint *)s_DIALOGUE_0045f388,local_10c,(uint *)&DAT_0045f380);
      local_114 = (FILE *)FUN_0044bc20((char *)local_218,&DAT_0045f394);
      if (local_114 == (FILE *)0x0) {
        local_8 = 1;
      }
      else {
        local_110 = FUN_0041199e(local_114);
        DAT_00483624 = (char *)FUN_0044c2f0(local_110 << 2);
        __fread_lk(DAT_00483624,local_110,4,local_114);
        _fclose(local_114);
      }
    }
    if (local_8 == 0) {
      FUN_0040c624(local_218,(uint *)s_PHONEMES_0045f3a0,local_10c,(uint *)&DAT_0045f398);
      DAT_0048242c = FUN_0044bc20((char *)local_218,&DAT_0045f3ac);
      if (DAT_0048242c == 0) {
        local_8 = 1;
      }
      else {
        FUN_0040c624(local_218,(uint *)s_DIALOGUE_0045f3b8,local_10c,(uint *)&DAT_0045f3b0);
        DAT_004859d0 = FUN_004554c0((LPCSTR)local_218,0x8000,unaff_EDI);
        if (DAT_004859d0 == -1) {
          local_8 = 1;
        }
      }
    }
    if (local_8 != 0) {
      FUN_0040ac43();
      FUN_0040ac38();
    }
  }
  else {
    local_8 = 1;
  }
  return local_8;
}

