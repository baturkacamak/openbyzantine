// rea decompile_all
// name: FUN_004221b1
// address: 0x4221b1
// body: available
// range: 0x4221b1-0x422317 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


int __cdecl FUN_004221b1(char *param_1,int param_2,int param_3)

{
  int iVar1;
  FILE *_File;
  char *pcVar2;
  undefined4 uVar3;
  int local_814;
  undefined4 local_808;
  uint local_804 [512];
  
  local_808 = 0;
  for (local_814 = 0; local_814 < param_3; local_814 = local_814 + 1) {
    *(undefined4 *)(param_2 + local_814 * 4) = 0;
  }
  _File = (FILE *)FUN_0044bc20(param_1,&DAT_0045fcf8);
  if (_File == (FILE *)0x0) {
    local_808 = 0;
  }
  else {
    do {
      if ((_File->_flag & 0x10) != 0) break;
      FUN_0044ce20((char *)local_804,0x800,_File);
      pcVar2 = FUN_0044c580(local_804);
      if (*(char *)((int)local_804 + (int)(pcVar2 + -1)) != '\n') break;
      *(char *)((int)local_804 + (int)(pcVar2 + -1)) = '\0';
      uVar3 = FUN_0044c2f0((SIZE_T)pcVar2);
      *(undefined4 *)(param_2 + local_808 * 4) = uVar3;
      iVar1 = local_808 * 4;
      local_808 = local_808 + 1;
      FUN_0044ba00(*(uint **)(param_2 + iVar1),local_804);
    } while (local_808 != param_3);
    _fclose(_File);
  }
  return local_808;
}

