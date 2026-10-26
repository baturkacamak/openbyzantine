// rea decompile_all
// name: FUN_0044d790
// address: 0x44d790
// body: available
// range: 0x44d790-0x44d8d7 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


uint __cdecl FUN_0044d790(uint param_1,FILE *param_2)

{
  uint uVar1;
  FILE *_File;
  byte bVar2;
  undefined3 extraout_var;
  undefined *puVar3;
  uint uVar4;
  uint uVar5;
  
  _File = param_2;
  uVar1 = param_2->_file;
  uVar4 = param_2->_flag;
  if (((uVar4 & 0x82) == 0) || ((uVar4 & 0x40) != 0)) {
    param_2->_flag = uVar4 | 0x20;
    return 0xffffffff;
  }
  if ((uVar4 & 1) != 0) {
    param_2->_cnt = 0;
    if ((param_2->_flag & 0x10U) == 0) {
      param_2->_flag = param_2->_flag | 0x20;
      return 0xffffffff;
    }
    param_2->_ptr = param_2->_base;
    param_2->_flag = param_2->_flag & 0xfffffffe;
  }
  uVar4 = param_2->_flag;
  uVar5 = 0;
  param_2->_flag = uVar4 | 2;
  param_2->_flag = uVar4 & 0xffffffef | 2;
  param_2->_cnt = 0;
  if ((param_2->_flag & 0x10cU) == 0) {
    if ((param_2 == (FILE *)&DAT_00462f20) || (param_2 == (FILE *)&DAT_00462f40)) {
      bVar2 = FUN_00454380(uVar1);
      if (CONCAT31(extraout_var,bVar2) != 0) goto LAB_0044d814;
    }
    __getbuf(_File);
  }
LAB_0044d814:
  if ((_File->_flag & 0x108U) == 0) {
    uVar4 = 1;
    uVar5 = FUN_0044d8e0(uVar1,(char *)&param_1,1);
  }
  else {
    uVar4 = (int)_File->_ptr - (int)_File->_base;
    _File->_ptr = _File->_base + 1;
    _File->_cnt = _File->_bufsiz + -1;
    if ((int)uVar4 < 1) {
      puVar3 = &DAT_004634e0;
      if (uVar1 != 0xffffffff) {
        puVar3 = (undefined *)
                 (*(int *)((int)&DAT_004a7720 + ((int)(uVar1 & 0xffffffe7) >> 3)) +
                 (uVar1 & 0x1f) * 8);
      }
      if ((puVar3[4] & 0x20) != 0) {
        FUN_00450fc0(uVar1,0,2);
      }
    }
    else {
      uVar5 = FUN_0044d8e0(uVar1,_File->_base,uVar4);
    }
    *_File->_base = (char)param_1;
  }
  if (uVar4 != uVar5) {
    _File->_flag = _File->_flag | 0x20;
    return 0xffffffff;
  }
  return param_1 & 0xff;
}

