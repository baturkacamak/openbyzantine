// rea decompile_all
// name: __getpath
// address: 0x455d00
// body: available
// range: 0x455d00-0x455d83 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


/* Library Function - Single Match
    __getpath
   
   Library: Visual Studio 1998 Release */

char * __cdecl __getpath(char *_Src,char *_Dst,size_t _SizeInChars)

{
  char cVar1;
  int iVar2;
  char *pcVar3;
  char *pcVar4;
  
  cVar1 = *_Src;
  while (cVar1 == ';') {
    _Src = _Src + 1;
    cVar1 = *_Src;
  }
  iVar2 = _SizeInChars - 1;
  pcVar3 = _Src;
  pcVar4 = _Src;
  if (iVar2 != 0) {
    cVar1 = *_Src;
    while ((cVar1 != '\0' && (cVar1 = *pcVar3, cVar1 != ';'))) {
      if (cVar1 == '\"') {
        pcVar3 = pcVar3 + 1;
        while ((cVar1 = *pcVar3, cVar1 != '\0' && (cVar1 != '\"'))) {
          *_Dst = cVar1;
          pcVar3 = pcVar3 + 1;
          _Dst = _Dst + 1;
          iVar2 = iVar2 + -1;
          pcVar4 = pcVar3;
          if (iVar2 == 0) goto LAB_00455d6d;
        }
        if (*pcVar3 == '\0') break;
        pcVar3 = pcVar3 + 1;
      }
      else {
        *_Dst = cVar1;
        pcVar3 = pcVar3 + 1;
        _Dst = _Dst + 1;
        iVar2 = iVar2 + -1;
        pcVar4 = pcVar3;
        if (iVar2 == 0) goto LAB_00455d6d;
      }
      cVar1 = *pcVar3;
    }
    cVar1 = *pcVar3;
    while (pcVar4 = _Src, cVar1 == ';') {
      pcVar3 = pcVar3 + 1;
      cVar1 = *pcVar3;
    }
  }
LAB_00455d6d:
  *_Dst = '\0';
  return (char *)((pcVar3 == pcVar4) - 1 & (uint)pcVar3);
}

