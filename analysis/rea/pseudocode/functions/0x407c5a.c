// rea decompile_all
// name: FUN_00407c5a
// address: 0x407c5a
// body: available
// range: 0x407c5a-0x407ca6 inclusive
// range: 0x407cac-0x407d50 inclusive
// range: 0x407d56-0x407d71 inclusive
// range: 0x407dcd-0x407df8 inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


bool __cdecl FUN_00407c5a(int param_1,char param_2)

{
  int local_8;
  
  switch(param_2) {
  case '\0':
  case '\x01':
  case '\x02':
  case '\x05':
  case '\b':
  case '\t':
  case '\f':
    if (param_1 == -1) {
      return true;
    }
    break;
  case '\x03':
  case '\x04':
  case '\x06':
  case '\a':
    return false;
  case '\n':
  case '\v':
  case '\r':
  case '\x0e':
  case '\x10':
    if (param_1 == -1) {
      if (DAT_0045d1c8 != 0) {
        return false;
      }
      return true;
    }
    break;
  case '\x0f':
  case '\x11':
  case '\x1d':
  case '\x1e':
    return false;
  case '\x12':
  case '\x13':
  case '\x14':
    if (param_1 == -1) {
      return false;
    }
    if (param_2 != '\x12') {
      local_8 = 0;
      while( true ) {
        if (*(int *)(*(int *)(&DAT_00486a10 + param_1 * 4) + 100) <= local_8) {
          return false;
        }
        if (*(int *)(&DAT_00486f90 +
                    *(int *)(*(int *)(&DAT_00486a10 + param_1 * 4) + local_8 * 4) * 4) ==
            DAT_0045d1c8) break;
        local_8 = local_8 + 1;
      }
      return true;
    }
    break;
  case '\x18':
  case '\x19':
  case '\x1a':
  case '\x1b':
    return false;
  }
  return *(int *)(&DAT_00486f90 + param_1 * 4) == DAT_0045d1c8;
}

