// rea decompile_all
// name: FUN_00404fe3
// address: 0x404fe3
// body: available
// range: 0x404fe3-0x40513e inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


MMRESULT __cdecl FUN_00404fe3(HMMIO param_1,uint param_2,int param_3,int param_4,uint *param_5)

{
  MMRESULT MVar1;
  uint local_5c;
  size_t local_58;
  _MMIOINFO local_54;
  MMRESULT local_c;
  uint local_8;
  
  local_c = 0;
  MVar1 = mmioGetInfo(param_1,&local_54,0);
  local_c = (MMRESULT)(MVar1 != 0);
  if (local_c == 0) {
    local_5c = param_2;
    if (*(uint *)(param_4 + 4) < param_2) {
      local_5c = *(uint *)(param_4 + 4);
    }
    *(int *)(param_4 + 4) = *(int *)(param_4 + 4) - local_5c;
    for (local_8 = 0; local_8 < local_5c; local_8 = local_8 + local_58) {
      if (local_54.pchNext == local_54.pchEndRead) {
        local_c = mmioAdvance(param_1,&local_54,0);
        if (local_c != 0) goto LAB_00405129;
        local_c = 0;
        if (local_54.pchNext == local_54.pchEndRead) {
          local_c = 0xe103;
          goto LAB_00405129;
        }
      }
      local_58 = (int)local_54.pchEndRead - (int)local_54.pchNext;
      if (local_5c - local_8 < local_58) {
        local_58 = local_5c - local_8;
      }
      FID_conflict__memcpy((void *)(local_8 + param_3),local_54.pchNext,local_58);
      local_54.pchNext = local_54.pchNext + local_58;
    }
    local_c = mmioSetInfo(param_1,&local_54,0);
    if (local_c == 0) {
      *param_5 = local_5c;
      return 0;
    }
  }
LAB_00405129:
  *param_5 = 0;
  return local_c;
}

