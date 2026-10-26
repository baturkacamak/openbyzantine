// rea decompile_all
// name: __mbschr
// address: 0x455c00
// body: available
// range: 0x455c00-0x455c8c inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


/* Library Function - Single Match
    __mbschr
   
   Library: Visual Studio 1998 Release */

uchar * __cdecl __mbschr(uchar *_Str,uint _Ch)

{
  byte bVar1;
  uint uVar2;
  uchar *puVar3;
  
  if (DAT_00463a1c == 0) {
    puVar3 = (uchar *)FUN_0044c520((char *)_Str,(char)_Ch);
    return puVar3;
  }
  bVar1 = *_Str;
  while (uVar2 = (uint)(ushort)bVar1, bVar1 != 0) {
    if ((*(byte *)((int)&DAT_00463918 + uVar2 + 1) & 4) == 0) {
      puVar3 = _Str;
      if (uVar2 == _Ch) break;
    }
    else {
      if (_Str[1] == '\0') {
        return (uchar *)0x0;
      }
      puVar3 = _Str + 1;
      if (CONCAT11(bVar1,_Str[1]) == _Ch) {
        return _Str;
      }
    }
    _Str = puVar3 + 1;
    bVar1 = puVar3[1];
  }
  return (uchar *)(-(uint)(uVar2 == _Ch) & (uint)_Str);
}

