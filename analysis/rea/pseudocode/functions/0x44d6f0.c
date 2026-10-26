// rea decompile_all
// name: _getenv
// address: 0x44d6f0
// body: available
// range: 0x44d6f0-0x44d785 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


/* Library Function - Single Match
    _getenv
   
   Library: Visual Studio 1998 Release */

char * __cdecl _getenv(char *_VarName)

{
  char cVar1;
  int iVar2;
  uint uVar3;
  uint uVar4;
  int *piVar5;
  char *pcVar6;
  
  if (DAT_00462c9c == (int *)0x0) {
    if ((DAT_00462ca4 != 0) && (iVar2 = ___wtomb_environ(), iVar2 != 0)) {
      return (char *)0x0;
    }
    if (DAT_00462c9c == (int *)0x0) {
      return (char *)0x0;
    }
  }
  if (_VarName != (char *)0x0) {
    uVar3 = 0xffffffff;
    pcVar6 = _VarName;
    do {
      if (uVar3 == 0) break;
      uVar3 = uVar3 - 1;
      cVar1 = *pcVar6;
      pcVar6 = pcVar6 + 1;
    } while (cVar1 != '\0');
    uVar3 = ~uVar3 - 1;
    iVar2 = *DAT_00462c9c;
    piVar5 = DAT_00462c9c;
    while (iVar2 != 0) {
      uVar4 = 0xffffffff;
      pcVar6 = (char *)*piVar5;
      do {
        if (uVar4 == 0) break;
        uVar4 = uVar4 - 1;
        cVar1 = *pcVar6;
        pcVar6 = pcVar6 + 1;
      } while (cVar1 != '\0');
      if (((uVar3 < ~uVar4 - 1) && (((uchar *)*piVar5)[uVar3] == '=')) &&
         (iVar2 = __mbsnbicoll((uchar *)*piVar5,(uchar *)_VarName,uVar3), iVar2 == 0)) {
        return (char *)(*piVar5 + 1 + uVar3);
      }
      piVar5 = piVar5 + 1;
      iVar2 = *piVar5;
    }
  }
  return (char *)0x0;
}

