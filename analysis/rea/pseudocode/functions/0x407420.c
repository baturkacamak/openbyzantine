// rea decompile_all
// name: FUN_00407420
// address: 0x407420
// body: available
// range: 0x407420-0x407526 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_00407420(int param_1)

{
  int iVar1;
  int local_18;
  short local_c;
  int local_8;
  
  local_8 = DAT_0045d1bc;
  local_c = 0xa0;
  FUN_004076b4(DAT_0045d1b4,0,0);
  for (local_18 = 0; (local_18 < 6 && (*(int *)(&DAT_00468cc0 + DAT_0045d1c4 * 4) != local_18));
      local_18 = local_18 + 1) {
    iVar1 = local_8 * 4;
    local_8 = local_8 + 1;
    iVar1 = *(int *)(&DAT_00486f90 + *(int *)(&DAT_00468d78 + iVar1 + DAT_0045d1c4 * 0x578) * 4);
    if ((iVar1 != DAT_0045d1c8) && (iVar1 != DAT_0045d1cc)) {
      if (*(int *)(iVar1 + 0xc) == 0) {
        FUN_00407254(iVar1);
      }
      FUN_004076b4(*(int **)(iVar1 + 0xc),local_c,6);
    }
    local_c = local_c + (short)DAT_0045d1ac;
  }
  FUN_004075fe(param_1);
  return;
}

