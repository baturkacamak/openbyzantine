// rea decompile_all
// name: FUN_00405a96
// address: 0x405a96
// body: available
// range: 0x405a96-0x405b5f inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


MMRESULT __cdecl
FUN_00405a96(LPSTR param_1,uint param_2,undefined4 param_3,short *param_4,int param_5)

{
  _MMCKINFO local_80;
  _MMCKINFO local_6c;
  _MMIOINFO local_58;
  HMMIO local_10;
  int local_c;
  MMRESULT local_8;
  
  local_8 = FUN_00405195(param_1,(int *)&local_10,param_4,&local_80,&local_6c);
  if (((local_8 == 0) && (local_8 = FUN_0040537d(&local_10,&local_80,&local_58), local_8 == 0)) &&
     (local_8 = FUN_004053fa(local_10,param_2,param_5,&local_80,&local_c,&local_58), local_8 == 0))
  {
    local_8 = FUN_00405499((int *)&local_10,&local_80,&local_6c,&local_58);
  }
  return local_8;
}

