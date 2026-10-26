// rea decompile_all
// name: FUN_00405499
// address: 0x405499
// body: available
// range: 0x405499-0x40562d inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


MMRESULT __cdecl
FUN_00405499(int *param_1,LPMMCKINFO param_2,LPMMCKINFO param_3,LPCMMIOINFO param_4)

{
  MMRESULT MVar1;
  MMRESULT local_8;
  
  if (*param_1 == 0) {
    local_8 = 0;
  }
  else {
    param_4->dwFlags = param_4->dwFlags | 0x10000000;
    local_8 = mmioSetInfo((HMMIO)*param_1,param_4,0);
    if (((local_8 == 0) && (local_8 = mmioAscend((HMMIO)*param_1,param_2,0), local_8 == 0)) &&
       (local_8 = mmioAscend((HMMIO)*param_1,param_3,0), local_8 == 0)) {
      mmioSeek((HMMIO)*param_1,0,0);
      local_8 = mmioDescend((HMMIO)*param_1,param_3,(MMCKINFO *)0x0,0);
      if (local_8 == 0) {
        param_2->ckid = 0x74636166;
        MVar1 = mmioDescend((HMMIO)*param_1,param_2,param_3,0x10);
        if (MVar1 == 0) {
          mmioWrite((HMMIO)*param_1,&stack0x00000014,4);
          mmioAscend((HMMIO)*param_1,param_2,0);
        }
        local_8 = mmioAscend((HMMIO)*param_1,param_3,0);
      }
    }
    if (*param_1 != 0) {
      mmioClose((HMMIO)*param_1,0);
      *param_1 = 0;
    }
  }
  return local_8;
}

