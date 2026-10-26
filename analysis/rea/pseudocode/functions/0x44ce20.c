// rea decompile_all
// name: FUN_0044ce20
// address: 0x44ce20
// body: available
// range: 0x44ce20-0x44ce81 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


char * __cdecl FUN_0044ce20(char *param_1,int param_2,FILE *param_3)

{
  int iVar1;
  uint uVar2;
  char *pcVar3;
  
  pcVar3 = param_1;
  if (param_2 < 1) {
    return (char *)0x0;
  }
  do {
    param_2 = param_2 + -1;
    if (param_2 == 0) break;
    iVar1 = param_3->_cnt + -1;
    param_3->_cnt = iVar1;
    if (iVar1 < 0) {
      uVar2 = __filbuf(param_3);
    }
    else {
      uVar2 = (uint)(byte)*param_3->_ptr;
      param_3->_ptr = param_3->_ptr + 1;
    }
    if (uVar2 == 0xffffffff) {
      if (pcVar3 == param_1) {
        return (char *)0x0;
      }
      break;
    }
    *pcVar3 = (char)uVar2;
    pcVar3 = pcVar3 + 1;
  } while ((char)uVar2 != '\n');
  *pcVar3 = '\0';
  return param_1;
}

