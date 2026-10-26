// rea decompile_all
// name: FUN_00422b04
// address: 0x422b04
// body: available
// range: 0x422b04-0x422c2c inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


undefined4 * __cdecl FUN_00422b04(LPSTR param_1,int param_2,int *param_3)

{
  bool bVar1;
  undefined4 *puVar2;
  int iVar3;
  HWND pHVar4;
  char *lpText;
  LPCSTR lpCaption;
  WPARAM wParam;
  UINT UVar5;
  LPARAM lParam;
  
  if (DAT_0045fd28 == -1) {
    iVar3 = FUN_00422f37(param_1,param_3);
    if (iVar3 == 0) {
      if (((param_3 == (int *)0x0) || (param_2 == -2)) || (param_3[10] == 0)) {
        bVar1 = false;
      }
      else {
        bVar1 = true;
      }
      if (bVar1) {
        (**(code **)(*DAT_0045fd14 + 0x3c))(DAT_0045fd14,0xfffff448);
      }
      DAT_0045fd18 = 1;
      DAT_0045fd28 = param_2;
      puVar2 = FUN_00428229(DAT_0045fd14,0);
      lParam = 0;
      wParam = 0x9c46;
      UVar5 = 0x111;
      pHVar4 = GetActiveWindow();
      SendMessageA(pHVar4,UVar5,wParam,lParam);
      if (bVar1) {
        FUN_004275ce(param_2,(int)puVar2,1);
        puVar2 = (undefined4 *)0x0;
      }
    }
    else {
      UVar5 = 0x30;
      lpCaption = (LPCSTR)0x0;
      lpText = s_Couldn_t_setup_spooling_0045fd30;
      pHVar4 = GetActiveWindow();
      MessageBoxA(pHVar4,lpText,lpCaption,UVar5);
      puVar2 = (undefined4 *)0x0;
    }
  }
  else {
    (**(code **)(*DAT_0045fd14 + 0x30))(DAT_0045fd14,0,0,1);
    puVar2 = (undefined4 *)0x0;
  }
  return puVar2;
}

