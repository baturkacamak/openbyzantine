// rea decompile_all
// name: _sscanf
// address: 0x44cf70
// body: available
// range: 0x44cf70-0x44cfb3 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


/* Library Function - Single Match
    _sscanf
   
   Library: Visual Studio 1998 Release */

int __cdecl _sscanf(char *_Src,char *_Format,...)

{
  char cVar1;
  int iVar2;
  uint uVar3;
  FILE local_20;
  
  uVar3 = 0xffffffff;
  local_20._flag = 0x49;
  local_20._base = _Src;
  local_20._ptr = _Src;
  do {
    if (uVar3 == 0) break;
    uVar3 = uVar3 - 1;
    cVar1 = *_Src;
    _Src = _Src + 1;
  } while (cVar1 != '\0');
  local_20._cnt = ~uVar3 - 1;
  iVar2 = FUN_00451d90(&local_20,(byte *)_Format,(undefined4 *)&stack0x0000000c);
  return iVar2;
}

