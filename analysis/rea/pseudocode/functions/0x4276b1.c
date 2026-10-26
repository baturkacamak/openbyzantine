// rea decompile_all
// name: FUN_004276b1
// address: 0x4276b1
// body: available
// range: 0x4276b1-0x4277ec inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_004276b1(int param_1)

{
  int *piVar1;
  int iVar2;
  int iVar3;
  HWND hWnd;
  float10 fVar4;
  float10 extraout_ST0;
  longlong lVar5;
  UINT Msg;
  WPARAM wParam;
  int local_c;
  
  piVar1 = *(int **)(*(int *)(param_1 + 0x10) + 8);
  iVar2 = *(int *)(param_1 + 8);
  local_c = -3000;
  if (DAT_00462ae8 == 0) {
    fVar4 = (float10)*(int *)(param_1 + 4) / (float10)50.0;
  }
  else {
    __adj_fdiv_m32(0x42480000);
    fVar4 = extraout_ST0;
  }
  if (DAT_00462ae8 != 0) {
    __adj_fdiv_m32((uint)(float)fVar4);
  }
  lVar5 = __ftol();
  while (local_c < iVar2 - (int)lVar5) {
    local_c = local_c + (int)lVar5;
    if (*(int *)(param_1 + 0xc) == 0) {
      (**(code **)(*piVar1 + 0x3c))(piVar1,local_c);
    }
    else {
      iVar3 = FUN_004228b5(local_c);
      if (iVar3 == -1) break;
    }
    Sleep(0x32);
  }
  if (*(int *)(param_1 + 0xc) == 0) {
    (**(code **)(*piVar1 + 0x3c))(piVar1,iVar2);
  }
  else {
    FUN_004228b5(iVar2);
  }
  wParam = 0xc353;
  Msg = 0x111;
  hWnd = (HWND)FUN_004161f6();
  PostMessageA(hWnd,Msg,wParam,param_1);
  return;
}

