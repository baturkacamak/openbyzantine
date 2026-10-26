// rea decompile_all
// name: FUN_00401074
// address: 0x401074
// body: available
// range: 0x401074-0x401149 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void FUN_00401074(void)

{
  HWND hWnd;
  char *lpText;
  LPCSTR lpCaption;
  UINT uType;
  int local_64;
  undefined4 local_60;
  char local_5c [80];
  int local_c;
  int local_8;
  
  local_c = (int)DAT_00486a0a;
  local_8 = DAT_00486a0c + -0x14;
  if ((((DAT_0045c060 != 0) && (DAT_00464880 != (void *)0x0)) && (-1 < DAT_00486a0a)) &&
     (((DAT_00486a0a < 0x281 && (0x13 < DAT_00486a0c)) && (DAT_00486a0c < 0x1a5)))) {
    FUN_004240ae(DAT_00464880,&local_c,&local_64);
    _sprintf(local_5c,s_X____d_Y____d_0045c094,local_64,local_60);
    uType = 0;
    lpCaption = (LPCSTR)0x0;
    lpText = local_5c;
    hWnd = (HWND)FUN_004161f6();
    MessageBoxA(hWnd,lpText,lpCaption,uType);
  }
  return;
}

