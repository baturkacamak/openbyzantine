// rea decompile_all
// name: FUN_00407ad5
// address: 0x407ad5
// body: available
// range: 0x407ad5-0x407b9b inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_00407ad5(undefined1 *param_1)

{
  int local_c;
  int local_8;
  
  FUN_00407b9c((int)param_1);
  for (local_8 = 0; local_8 < 10; local_8 = local_8 + 1) {
    for (local_c = 0; local_c < *(int *)(&DAT_00468cc0 + local_8 * 4); local_c = local_c + 1) {
      if (**(char **)(&DAT_00486f90 + *(int *)(&DAT_00468d78 + local_c * 4 + local_8 * 0x578) * 4)
          == '\x03') {
        **(undefined1 **)
          (&DAT_00486f90 + *(int *)(&DAT_00468d78 + local_c * 4 + local_8 * 0x578) * 4) = 1;
      }
    }
  }
  *param_1 = 3;
  FUN_00407420(1);
  return;
}

