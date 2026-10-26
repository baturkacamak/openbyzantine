// rea decompile_all
// name: FUN_0044bf20
// address: 0x44bf20
// body: available
// range: 0x44bf20-0x44bf8a inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


int __cdecl FUN_0044bf20(int param_1)

{
  FILE *_File;
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  
  iVar2 = 0;
  iVar4 = 0;
  iVar3 = 0;
  do {
    _File = *(FILE **)(DAT_004a782c + iVar4);
    if ((_File != (FILE *)0x0) && ((_File->_flag & 0x83U) != 0)) {
      if (param_1 == 1) {
        iVar1 = _fflush(_File);
        if (iVar1 != -1) {
          iVar2 = iVar2 + 1;
        }
      }
      else if ((param_1 == 0) && ((_File->_flag & 2U) != 0)) {
        iVar1 = _fflush(_File);
        if (iVar1 == -1) {
          iVar3 = -1;
        }
      }
    }
    iVar4 = iVar4 + 4;
  } while (iVar4 < 0x800);
  if (param_1 != 1) {
    iVar2 = iVar3;
  }
  return iVar2;
}

