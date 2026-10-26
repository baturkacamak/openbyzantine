// rea decompile_all
// name: FUN_0042b679
// address: 0x42b679
// body: available
// range: 0x42b679-0x42b736 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl
FUN_0042b679(int param_1,int param_2,uint param_3,uint param_4,uint param_5,uint param_6,
            uint param_7,uint param_8,uint param_9,uint param_10)

{
  undefined4 local_14;
  undefined4 local_10;
  undefined4 local_c;
  
  local_10 = (void *)((param_7 & 0xffff) * 2 + (param_8 & 0xffff) * (param_5 & 0xffff) * 2 + param_1
                     );
  local_c = (void *)((param_3 & 0xffff) * 2 + (param_6 & 0xffff) * (param_4 & 0xffff) * 2 + param_2)
  ;
  for (local_14 = 0; local_14 < (int)(param_10 & 0xffff); local_14 = local_14 + 1) {
    FID_conflict__memcpy(local_10,local_c,(param_9 & 0xffff) * 2);
    local_c = (void *)((int)local_c + (param_6 & 0xffff) * 2);
    local_10 = (void *)((int)local_10 + (param_5 & 0xffff) * 2);
  }
  return;
}

