// rea decompile_all
// name: FUN_0040aa42
// address: 0x40aa42
// body: available
// range: 0x40aa42-0x40ab26 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_0040aa42(uint *param_1,int param_2)

{
  bool bVar1;
  int local_28;
  int local_24;
  undefined1 local_18;
  undefined1 local_17;
  
  local_24 = 0;
  bVar1 = false;
  if (param_2 < 0) {
    bVar1 = true;
    param_2 = -param_2;
  }
  do {
    for (local_28 = local_24; 0 < local_28; local_28 = local_28 + -1) {
      *(undefined1 *)(local_28 + (int)param_1) = *(undefined1 *)(local_28 + -1 + (int)param_1);
    }
    *(char *)param_1 = (char)param_2 + (char)(param_2 / 10) * -10 + '0';
    param_2 = param_2 / 10;
    local_24 = local_24 + 1;
  } while (param_2 != 0);
  *(undefined1 *)(local_24 + (int)param_1) = 0;
  if (bVar1) {
    local_18 = 0x2d;
    local_17 = 0;
    FUN_0044ba08((uint *)&local_18,param_1);
    FUN_0044ba00(param_1,(uint *)&local_18);
  }
  return;
}

