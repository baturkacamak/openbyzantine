// rea decompile_all
// name: FUN_00407933
// address: 0x407933
// body: available
// range: 0x407933-0x407ad4 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_00407933(uint param_1,uint param_2)

{
  char *pcVar1;
  int local_10;
  int local_c;
  
  pcVar1 = *(char **)(&DAT_00486f90 + (param_2 & 0xffff) * 4);
  if (((pcVar1 == (char *)0x0) || (*pcVar1 == '\0')) || (*pcVar1 == '\x02')) {
    return;
  }
  if (*pcVar1 == '\x03') {
    FUN_00407b9c(0);
  }
  local_c = 0;
  while( true ) {
    if (*(int *)(&DAT_00468cc0 + (param_1 & 0xffff) * 4) <= local_c) goto LAB_00407a81;
    if (*(uint *)(&DAT_00468d78 + (param_1 & 0xffff) * 0x578 + local_c * 4) == (param_2 & 0xffff))
    break;
    local_c = local_c + 1;
  }
  for (local_10 = local_c; local_10 < *(int *)(&DAT_00468cc0 + (param_1 & 0xffff) * 4) + -1;
      local_10 = local_10 + 1) {
    *(undefined4 *)(&DAT_00468d78 + (param_1 & 0xffff) * 0x578 + local_10 * 4) =
         *(undefined4 *)(local_10 * 4 + 0x468d7c + (param_1 & 0xffff) * 0x578);
  }
  *(int *)(&DAT_00468cc0 + (param_1 & 0xffff) * 4) =
       *(int *)(&DAT_00468cc0 + (param_1 & 0xffff) * 4) + -1;
LAB_00407a81:
  if ((DAT_0045d1bc != 0) && (*(int *)(&DAT_00468cc0 + (param_1 & 0xffff) * 4) < DAT_0045d1bc + 6))
  {
    DAT_0045d1bc = DAT_0045d1bc + -1;
  }
  if ((param_1 & 0xffff) != DAT_0045d1c4) {
    return;
  }
  FUN_00407420(1);
  return;
}

