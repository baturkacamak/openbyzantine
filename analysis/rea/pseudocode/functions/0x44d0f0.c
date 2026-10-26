// rea decompile_all
// name: __filbuf
// address: 0x44d0f0
// body: available
// range: 0x44d0f0-0x44d1df inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


/* Library Function - Single Match
    __filbuf
   
   Library: Visual Studio 1998 Release */

int __cdecl __filbuf(FILE *_File)

{
  uint uVar1;
  byte *pbVar2;
  int iVar3;
  undefined *puVar4;
  
  uVar1 = _File->_flag;
  if (((uVar1 & 0x83) == 0) || ((uVar1 & 0x40) != 0)) {
    return -1;
  }
  if ((uVar1 & 2) != 0) {
    _File->_flag = uVar1 | 0x20;
    return -1;
  }
  _File->_flag = uVar1 | 1;
  if ((uVar1 & 0x10c) == 0) {
    __getbuf(_File);
  }
  else {
    _File->_ptr = _File->_base;
  }
  iVar3 = FUN_0044e600(_File->_file,_File->_base,_File->_bufsiz);
  _File->_cnt = iVar3;
  if ((iVar3 != 0) && (iVar3 != -1)) {
    if ((_File->_flag & 0x82U) == 0) {
      uVar1 = _File->_file;
      puVar4 = &DAT_004634e0;
      if (uVar1 != 0xffffffff) {
        puVar4 = (undefined *)
                 (*(int *)((int)&DAT_004a7720 + ((int)(uVar1 & 0xffffffe7) >> 3)) +
                 (uVar1 & 0x1f) * 8);
      }
      if ((puVar4[4] & 0x82) == 0x82) {
        _File->_flag = _File->_flag | 0x2000;
      }
    }
    if (((_File->_bufsiz == 0x200) && ((_File->_flag & 8U) != 0)) && ((_File->_flag & 0x400U) == 0))
    {
      _File->_bufsiz = 0x1000;
    }
    _File->_cnt = _File->_cnt + -1;
    pbVar2 = (byte *)_File->_ptr;
    _File->_ptr = (char *)(pbVar2 + 1);
    return (uint)*pbVar2;
  }
  _File->_flag = _File->_flag | (-(uint)(iVar3 == 0) & 0xfffffff0) + 0x20;
  _File->_cnt = 0;
  return -1;
}

