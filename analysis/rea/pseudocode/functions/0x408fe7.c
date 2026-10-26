// rea decompile_all
// name: FUN_00408fe7
// address: 0x408fe7
// body: available
// range: 0x408fe7-0x4090c3 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void FUN_00408fe7(void)

{
  int local_20;
  int local_1c;
  int local_18 [5];
  
  local_18[0] = 0;
  local_18[1] = 0xb5;
  local_18[2] = 0x144;
  local_18[3] = 500;
  local_18[4] = 0x280;
  local_20 = (int)DAT_00486a0a;
  if (0x27f < local_20) {
    local_20 = 0x27f;
  }
  if (DAT_0045d1d0 != 0) {
    for (local_1c = 0; (local_1c < 5 && (local_18[local_1c] <= local_20)); local_1c = local_1c + 1)
    {
    }
    if (local_1c != DAT_0045d1d4) {
      FUN_00408f32(1);
      DAT_0045d1d4 = local_1c;
      FUN_004090c4(local_1c,1);
      FUN_0040df05(0x1c);
      DAT_00483050 = 0xffff;
    }
  }
  return;
}

