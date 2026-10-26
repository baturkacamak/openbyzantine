// rea decompile_all
// name: FUN_00438afa
// address: 0x438afa
// body: available
// range: 0x438afa-0x438c7f inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


undefined4 FUN_00438afa(void)

{
  char *_Str1;
  int iVar1;
  undefined4 uVar2;
  
  _Str1 = &DAT_00483bc0 + (uint)DAT_00482428 * 9;
  iVar1 = __strcmpi(_Str1,&DAT_004617b4);
  if (iVar1 == 0) {
    uVar2 = 5;
  }
  else {
    iVar1 = __strcmpi(_Str1,&DAT_004617b8);
    if (iVar1 == 0) {
      uVar2 = 4;
    }
    else {
      iVar1 = __strcmpi(_Str1,&DAT_004617bc);
      if ((iVar1 != 0) && (iVar1 = __strcmpi(_Str1,s_hstmed_004617c0), iVar1 != 0)) {
        iVar1 = __strcmpi(_Str1,&DAT_004617c8);
        if ((iVar1 == 0) ||
           ((iVar1 = __strcmpi(_Str1,s_lintel_004617cc), iVar1 == 0 ||
            (iVar1 = __strcmpi(_Str1,s_morris_004617d4), iVar1 == 0)))) {
          return 3;
        }
        iVar1 = __strcmpi(_Str1,&DAT_004617dc);
        if ((iVar1 != 0) &&
           ((((iVar1 = __strcmpi(_Str1,s_clock_004617e0), iVar1 != 0 &&
              (iVar1 = __strcmpi(_Str1,s_voice_004617e8), iVar1 != 0)) &&
             (iVar1 = __strcmpi(_Str1,&DAT_004617f0), iVar1 != 0)) &&
            (iVar1 = __strcmpi(_Str1,&DAT_004617f8), iVar1 != 0)))) {
          return 0;
        }
        return 1;
      }
      uVar2 = 2;
    }
  }
  return uVar2;
}

