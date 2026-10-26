// rea decompile_all
// name: __getstream
// address: 0x44fe40
// body: available
// range: 0x44fe40-0x44febd inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


/* Library Function - Single Match
    __getstream
   
   Library: Visual Studio 1998 Release */

FILE * __cdecl __getstream(void)

{
  int iVar1;
  int *piVar2;
  FILE *pFVar3;
  int iVar4;
  
  pFVar3 = (FILE *)0x0;
  iVar4 = 0;
  piVar2 = DAT_004a782c;
  if (0 < DAT_004a7828) {
    do {
      if (*piVar2 == 0) {
        iVar1 = FUN_0044c2f0(0x20);
        DAT_004a782c[iVar4] = iVar1;
        if ((FILE *)DAT_004a782c[iVar4] != (FILE *)0x0) {
          pFVar3 = (FILE *)DAT_004a782c[iVar4];
        }
        break;
      }
      if ((*(uint *)(*piVar2 + 0xc) & 0x83) == 0) {
        pFVar3 = (FILE *)DAT_004a782c[iVar4];
        break;
      }
      iVar4 = iVar4 + 1;
      piVar2 = piVar2 + 1;
    } while (iVar4 < DAT_004a7828);
  }
  if (pFVar3 != (FILE *)0x0) {
    pFVar3->_cnt = 0;
    pFVar3->_flag = 0;
    pFVar3->_base = (char *)0x0;
    pFVar3->_ptr = (char *)0x0;
    pFVar3->_tmpfname = (char *)0x0;
    pFVar3->_file = -1;
  }
  return pFVar3;
}

