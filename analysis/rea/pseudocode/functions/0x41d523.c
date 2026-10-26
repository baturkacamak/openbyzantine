// rea decompile_all
// name: FUN_0041d523
// address: 0x41d523
// body: available
// range: 0x41d523-0x41d670 inclusive
// range: 0x41d676-0x41d694 inclusive
// range: 0x41d6a8-0x41d6ac inclusive
// range: 0x41d6b2-0x41d6de inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


undefined4 FUN_0041d523(HWND param_1,int param_2,ushort param_3,int param_4)

{
  HWND hWnd;
  uint *lpString;
  uint local_58 [20];
  HWND local_8;
  
  if (param_2 == 0x110) {
    FUN_0041d45a(local_58,param_4);
    lpString = local_58;
    hWnd = GetDlgItem(param_1,0x3f9);
    SetWindowTextA(hWnd,(LPCSTR)lpString);
    local_8 = GetDlgItem(param_1,6);
    if (local_8 != (HWND)0x0) {
      FUN_0041d45a(local_58,0x4c);
      SetWindowTextA(local_8,(LPCSTR)local_58);
    }
    local_8 = GetDlgItem(param_1,7);
    if (local_8 != (HWND)0x0) {
      FUN_0041d45a(local_58,0x4d);
      SetWindowTextA(local_8,(LPCSTR)local_58);
    }
    local_8 = GetDlgItem(param_1,1);
    if (local_8 != (HWND)0x0) {
      FUN_0041d45a(local_58,0x4e);
      SetWindowTextA(local_8,(LPCSTR)local_58);
    }
    local_8 = GetDlgItem(param_1,2);
    if (local_8 != (HWND)0x0) {
      FUN_0041d45a(local_58,0x4f);
      SetWindowTextA(local_8,(LPCSTR)local_58);
    }
    SetFocus(param_1);
  }
  else if (param_2 == 0x111) {
    switch(param_3) {
    case 1:
    case 2:
    case 6:
    case 7:
      EndDialog(param_1,(uint)param_3);
      return 1;
    default:
    }
  }
  return 0;
}

