// rea decompile_all
// name: FUN_0040ede0
// address: 0x40ede0
// body: available
// range: 0x40ede0-0x40eec2 inclusive
// range: 0x40eec8-0x40eee5 inclusive
// range: 0x40eeeb-0x40ef17 inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


undefined4 FUN_0040ede0(HWND param_1,int param_2,short param_3)

{
  HWND pHVar1;
  char *lpString;
  LPCSTR pCVar2;
  int iVar3;
  char local_8 [4];
  
  if (param_2 == 0x110) {
    pCVar2 = &DAT_0046c470;
    pHVar1 = GetDlgItem(param_1,0x3ea);
    SetWindowTextA(pHVar1,pCVar2);
    pCVar2 = &DAT_00486a08;
    pHVar1 = GetDlgItem(param_1,0x3eb);
    SetWindowTextA(pHVar1,pCVar2);
    SetFocus(param_1);
  }
  else if (param_2 == 0x111) {
    if (param_3 == 1) {
      iVar3 = 2;
      lpString = local_8;
      pHVar1 = GetDlgItem(param_1,0x3eb);
      GetWindowTextA(pHVar1,lpString,iVar3);
      iVar3 = FID_conflict___toupper_lk((int)local_8[0]);
      local_8[0] = (char)iVar3;
      if (('@' < local_8[0]) && (local_8[0] < '[')) {
        DAT_00486a08 = local_8[0];
        EndDialog(param_1,1);
      }
      return 1;
    }
    if (param_3 == 2) {
      EndDialog(param_1,0);
      return 1;
    }
  }
  return 0;
}

