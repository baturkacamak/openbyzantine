// rea decompile_all
// name: FUN_0040c9ce
// address: 0x40c9ce
// body: available
// range: 0x40c9ce-0x40cb25 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


char __cdecl FUN_0040c9ce(int param_1)

{
  char cVar1;
  char cVar2;
  int iVar3;
  int local_10;
  char local_c;
  
  cVar1 = DAT_00486a0e;
  local_c = '\0';
  if (((DAT_0045da74 == 1) || (DAT_0045da74 == 2)) && (param_1 != 0)) {
    DAT_0045da78 = param_1;
  }
  if ((DAT_00486a0e == '\0') || (iVar3 = FUN_0040c70e(param_1), cVar2 = DAT_00486a0e, iVar3 == 0)) {
    for (local_10 = 0; cVar2 = local_c, DAT_00486a0e = cVar1, local_10 < DAT_0045da7c;
        local_10 = local_10 + 1) {
      DAT_00486a0e = DAT_0046c440 + (char)local_10;
      if (cVar1 != DAT_00486a0e) {
        if ((DAT_0045da74 != 0) && (0 < local_10)) {
          DAT_00486a0e = DAT_0045e768;
        }
        if (DAT_0045da74 == 2) {
          if (param_1 == 3) {
            DAT_00486a0e = 'C';
          }
          else if ((param_1 == 4) || (param_1 == 5)) {
            DAT_00486a0e = 'D';
          }
          else {
            DAT_00486a0e = 'E';
          }
        }
        iVar3 = FUN_0040c70e(param_1);
        if (iVar3 != 0) {
          cVar2 = DAT_00486a0e;
          DAT_00486a0e = cVar1;
          return cVar2;
        }
      }
    }
  }
  return cVar2;
}

