// rea decompile_all
// name: __fread_lk
// address: 0x44a650
// body: available
// range: 0x44a650-0x44a795 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


/* Library Function - Single Match
    __fread_lk
   
   Library: Visual Studio 1998 Release */

uint __cdecl __fread_lk(char *param_1,uint param_2,uint param_3,FILE *param_4)

{
  uint uVar1;
  int iVar2;
  uint uVar3;
  uint uVar4;
  uint uVar5;
  char *pcVar6;
  char *pcVar7;
  uint local_4;
  
  uVar1 = param_2 * param_3;
  if (uVar1 == 0) {
    return 0;
  }
  uVar5 = uVar1;
  if ((param_4->_flag & 0x10cU) == 0) {
    local_4 = 0x1000;
  }
  else {
    local_4 = param_4->_bufsiz;
  }
  while( true ) {
    while( true ) {
      while( true ) {
        if (uVar5 == 0) {
          return param_3;
        }
        if (((param_4->_flag & 0x10cU) == 0) || (uVar3 = param_4->_cnt, uVar3 == 0)) break;
        uVar4 = uVar5;
        if (uVar3 <= uVar5) {
          uVar4 = uVar3;
        }
        uVar5 = uVar5 - uVar4;
        pcVar6 = param_4->_ptr;
        pcVar7 = param_1;
        for (uVar3 = uVar4 >> 2; uVar3 != 0; uVar3 = uVar3 - 1) {
          *(undefined4 *)pcVar7 = *(undefined4 *)pcVar6;
          pcVar6 = pcVar6 + 4;
          pcVar7 = pcVar7 + 4;
        }
        for (uVar3 = uVar4 & 3; uVar3 != 0; uVar3 = uVar3 - 1) {
          *pcVar7 = *pcVar6;
          pcVar6 = pcVar6 + 1;
          pcVar7 = pcVar7 + 1;
        }
        param_4->_cnt = param_4->_cnt - uVar4;
        param_4->_ptr = param_4->_ptr + uVar4;
        param_1 = param_1 + uVar4;
      }
      if (local_4 <= uVar5) break;
      iVar2 = __filbuf(param_4);
      if (iVar2 == -1) {
        return (uVar1 - uVar5) / param_2;
      }
      uVar5 = uVar5 - 1;
      *param_1 = (char)iVar2;
      local_4 = param_4->_bufsiz;
      param_1 = param_1 + 1;
    }
    uVar3 = uVar5;
    if (local_4 != 0) {
      uVar3 = uVar5 - uVar5 % local_4;
    }
    iVar2 = FUN_0044e600(param_4->_file,param_1,uVar3);
    if (iVar2 == 0) break;
    if (iVar2 == -1) {
      param_4->_flag = param_4->_flag | 0x20;
      return (uVar1 - uVar5) / param_2;
    }
    uVar5 = uVar5 - iVar2;
    param_1 = param_1 + iVar2;
  }
  param_4->_flag = param_4->_flag | 0x10;
  return (uVar1 - uVar5) / param_2;
}

