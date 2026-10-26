// rea decompile_all
// name: _mbtowc
// address: 0x4566b0
// body: available
// range: 0x4566b0-0x4567b2 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


/* Library Function - Single Match
    _mbtowc
   
   Library: Visual Studio 1998 Release */

int __cdecl _mbtowc(wchar_t *_DstCh,char *_SrcCh,size_t _SrcSizeInBytes)

{
  byte bVar1;
  int iVar2;
  
  if ((_SrcCh == (char *)0x0) || (_SrcSizeInBytes == 0)) {
    return 0;
  }
  bVar1 = *_SrcCh;
  if (bVar1 == 0) {
    if (_DstCh != (wchar_t *)0x0) {
      *_DstCh = L'\0';
    }
    return 0;
  }
  if (DAT_00463548 != 0) {
    if ((PTR_DAT_00462ce0[(uint)bVar1 * 2 + 1] & 0x80) == 0) {
      iVar2 = MultiByteToWideChar(DAT_00463558,9,_SrcCh,1,_DstCh,(uint)(_DstCh != (wchar_t *)0x0));
      if (iVar2 != 0) {
        return 1;
      }
      DAT_00462c74 = 0x2a;
      return -1;
    }
    if (((((int)DAT_00462eec < 2) || ((int)_SrcSizeInBytes < (int)DAT_00462eec)) ||
        (iVar2 = MultiByteToWideChar(DAT_00463558,9,_SrcCh,DAT_00462eec,_DstCh,
                                     (uint)(_DstCh != (wchar_t *)0x0)), iVar2 == 0)) &&
       ((_SrcSizeInBytes < DAT_00462eec || (_SrcCh[1] == '\0')))) {
      DAT_00462c74 = 0x2a;
      return -1;
    }
    return DAT_00462eec;
  }
  if (_DstCh != (wchar_t *)0x0) {
    *_DstCh = (ushort)bVar1;
  }
  return 1;
}

