// rea decompile_all
// name: FUN_0040ef18
// address: 0x40ef18
// body: available
// range: 0x40ef18-0x40f074 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


INT_PTR __cdecl FUN_0040ef18(HWND param_1)

{
  FILE *pFVar1;
  INT_PTR IVar2;
  
  pFVar1 = (FILE *)FUN_0044bc20(s_drives_btb_0045e770,&DAT_0045e76c);
  if (pFVar1 != (FILE *)0x0) {
    __fread_lk(&DAT_0045e764,1,1,pFVar1);
    __fread_lk(&DAT_0045e768,1,1,pFVar1);
    _fclose(pFVar1);
  }
  FUN_0044ba00((uint *)&DAT_0046c470,(uint *)s_Columbus1_0045e77c);
  DAT_00486a08 = DAT_0045e764;
  IVar2 = DialogBoxParamA((HINSTANCE)0x0,(LPCSTR)0x6f,param_1,FUN_0040ede0,0);
  if (IVar2 == 0) {
    IVar2 = 0;
  }
  else {
    DAT_0045e764 = DAT_00486a08;
    FUN_0044ba00((uint *)&DAT_0046c470,(uint *)s_Columbus2_0045e788);
    DAT_00486a08 = DAT_0045e768;
    IVar2 = DialogBoxParamA((HINSTANCE)0x0,(LPCSTR)0x6f,param_1,FUN_0040ede0,0);
    if (IVar2 != 0) {
      DAT_0045e768 = DAT_00486a08;
    }
    pFVar1 = (FILE *)FUN_0044bc20(s_drives_btb_0045e798,&DAT_0045e794);
    if (pFVar1 != (FILE *)0x0) {
      FUN_0044a460(&DAT_0045e764,1,1,pFVar1);
      FUN_0044a460(&DAT_0045e768,1,1,pFVar1);
      _fclose(pFVar1);
    }
  }
  return IVar2;
}

