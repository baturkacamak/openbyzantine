// rea decompile_all
// name: FID_conflict:_ungetc
// address: 0x456af0
// body: available
// range: 0x456af0-0x456b77 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


/* Library Function - Multiple Matches With Different Base Names
    __ungetc_lk
    _ungetc
   
   Library: Visual Studio 1998 Release */

int __cdecl FID_conflict__ungetc(int _Ch,FILE *_File)

{
  char *pcVar1;
  uint uVar2;
  
  if (_Ch != -1) {
    uVar2 = _File->_flag;
    if (((uVar2 & 1) != 0) || (((uVar2 & 0x80) != 0 && ((uVar2 & 2) == 0)))) {
      if (_File->_base == (char *)0x0) {
        __getbuf(_File);
      }
      if (_File->_base == _File->_ptr) {
        if (_File->_cnt != 0) {
          return -1;
        }
        _File->_ptr = _File->_ptr + 1;
      }
      pcVar1 = _File->_ptr;
      if ((_File->_flag & 0x40) == 0) {
        _File->_ptr = pcVar1 + -1;
        pcVar1[-1] = (char)_Ch;
      }
      else {
        _File->_ptr = pcVar1 + -1;
        if (pcVar1[-1] != (char)_Ch) {
          _File->_ptr = pcVar1;
          return -1;
        }
      }
      _File->_cnt = _File->_cnt + 1;
      uVar2 = _File->_flag & 0xffffffef;
      _File->_flag = uVar2;
      _File->_flag = uVar2 | 1;
      return _Ch & 0xff;
    }
  }
  return -1;
}

