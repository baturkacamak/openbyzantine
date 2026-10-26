// rea decompile_all
// name: __openfile
// address: 0x44fc30
// body: available
// range: 0x44fc30-0x44fdca inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* Library Function - Single Match
    __openfile
   
   Library: Visual Studio 1998 Release */

FILE * __cdecl __openfile(char *_Filename,char *_Mode,int _ShFlag,FILE *_File)

{
  char cVar1;
  bool bVar2;
  bool bVar3;
  bool bVar4;
  char *pcVar5;
  uint uVar6;
  uint uVar7;
  
  bVar3 = false;
  cVar1 = *_Mode;
  bVar4 = false;
  if (cVar1 == 'a') {
    uVar6 = 0x109;
  }
  else {
    if (cVar1 == 'r') {
      uVar6 = 0;
      uVar7 = DAT_00463eb8 | 1;
      goto LAB_0044fc7d;
    }
    if (cVar1 != 'w') {
      return (FILE *)0x0;
    }
    uVar6 = 0x301;
  }
  uVar7 = DAT_00463eb8 | 2;
LAB_0044fc7d:
  bVar2 = true;
  pcVar5 = _Mode + 1;
  cVar1 = *pcVar5;
  while ((cVar1 != '\0' && (bVar2))) {
    switch(*pcVar5) {
    case '+':
      if ((uVar6 & 2) == 0) {
        uVar6 = uVar6 & 0xfffffffe | 2;
        uVar7 = uVar7 & 0xfffffffc | 0x80;
      }
      else {
        bVar2 = false;
      }
      break;
    default:
      bVar2 = false;
      break;
    case 'D':
      if ((uVar6 & 0x40) == 0) {
        uVar6 = uVar6 | 0x40;
      }
      else {
        bVar2 = false;
      }
      break;
    case 'R':
      if (bVar3) {
        bVar2 = false;
      }
      else {
        bVar3 = true;
        uVar6 = uVar6 | 0x10;
      }
      break;
    case 'S':
      if (bVar3) {
        bVar2 = false;
      }
      else {
        bVar3 = true;
        uVar6 = uVar6 | 0x20;
      }
      break;
    case 'T':
      if ((uVar6 & 0x1000) == 0) {
        uVar6 = uVar6 | 0x1000;
      }
      else {
        bVar2 = false;
      }
      break;
    case 'b':
      if ((uVar6 & 0xc000) == 0) {
        uVar6 = uVar6 | 0x8000;
      }
      else {
        bVar2 = false;
      }
      break;
    case 'c':
      if (bVar4) {
        bVar2 = false;
      }
      else {
        bVar4 = true;
        uVar7 = uVar7 | 0x4000;
      }
      break;
    case 'n':
      if (bVar4) {
        bVar2 = false;
      }
      else {
        bVar4 = true;
        uVar7 = uVar7 & 0xffffbfff;
      }
      break;
    case 't':
      if ((uVar6 & 0xc000) == 0) {
        uVar6 = uVar6 | 0x4000;
      }
      else {
        bVar2 = false;
      }
    }
    pcVar5 = pcVar5 + 1;
    cVar1 = *pcVar5;
  }
  uVar6 = FUN_004554e0(_Filename,uVar6,_ShFlag,0x1a4);
  if (-1 < (int)uVar6) {
    _DAT_00463180 = _DAT_00463180 + 1;
    _File->_flag = uVar7;
    _File->_cnt = 0;
    _File->_ptr = (char *)0x0;
    _File->_base = (char *)0x0;
    _File->_tmpfname = (char *)0x0;
    _File->_file = uVar6;
    return _File;
  }
  return (FILE *)0x0;
}

