// rea decompile_all
// name: FUN_0041c36f
// address: 0x41c36f
// body: available
// range: 0x41c36f-0x41c48a inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __fastcall FUN_0041c36f(int *param_1)

{
  int local_8;
  
  for (local_8 = 0; local_8 < 0x4c; local_8 = local_8 + 1) {
    if (param_1[local_8 + 1] != 0) {
      FUN_0044ac40((LPVOID)param_1[local_8 + 1]);
    }
  }
  if (param_1[0x9a] != 0) {
    (**(code **)(*(int *)param_1[0x9a] + 8))(param_1[0x9a]);
  }
  if (*param_1 != 0) {
    FUN_0044ac40((LPVOID)*param_1);
  }
  if (param_1[0x9c] != 0) {
    AVIStreamRelease(param_1[0x9c]);
  }
  if (param_1[0x9b] != 0) {
    FUN_0041b152(param_1[0x9b]);
    if ((void *)param_1[0x9b] != (void *)0x0) {
      FUN_0041cde0((void *)param_1[0x9b],1);
    }
  }
  if (param_1[0xa0] != 0) {
    DAT_0045d1d0 = 1;
  }
  return;
}

