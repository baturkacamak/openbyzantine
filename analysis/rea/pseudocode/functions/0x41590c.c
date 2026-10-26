// rea decompile_all
// name: FUN_0041590c
// address: 0x41590c
// body: available
// range: 0x41590c-0x415a4d inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


undefined4 __cdecl FUN_0041590c(HINSTANCE param_1)

{
  ATOM AVar1;
  undefined4 uVar2;
  int iVar3;
  int iVar4;
  HWND pHVar5;
  HMENU pHVar6;
  LPVOID pvVar7;
  WNDCLASSA local_30;
  uint local_8;
  
  local_30.style = 3;
  local_30.lpfnWndProc = FUN_00414cd2;
  local_30.cbClsExtra = 0;
  local_30.cbWndExtra = 0;
  local_30.hInstance = param_1;
  local_30.hIcon = (HICON)0x0;
  local_30.hCursor = LoadCursorA((HINSTANCE)0x0,(LPCSTR)0x7f00);
  local_30.hbrBackground = GetStockObject(4);
  local_30.lpszMenuName = s_Byzantine_0045f0d4;
  local_30.lpszClassName = s_Byzantine_0045f0e0;
  AVar1 = RegisterClassA(&local_30);
  local_8 = (uint)AVar1;
  if (local_8 == 0) {
    uVar2 = 0;
  }
  else {
    if (DAT_0045f080 == 0) {
      pvVar7 = (LPVOID)0x0;
      pHVar6 = (HMENU)0x0;
      pHVar5 = (HWND)0x0;
      iVar3 = GetSystemMetrics(1);
      iVar4 = GetSystemMetrics(0);
      DAT_0046c4e8 = CreateWindowExA(8,s_Byzantine_0045f108,s_Byzantine_0045f0fc,0x90000000,0,0,
                                     iVar4,iVar3,pHVar5,pHVar6,param_1,pvVar7);
    }
    else {
      pvVar7 = (LPVOID)0x0;
      pHVar6 = (HMENU)0x0;
      pHVar5 = (HWND)0x0;
      iVar3 = GetSystemMetrics(1);
      iVar4 = GetSystemMetrics(0);
      DAT_0046c4e8 = CreateWindowExA(8,s_Byzantine_0045f0f0,&DAT_0045f0ec,0x90000000,0,0,iVar4,iVar3
                                     ,pHVar5,pHVar6,param_1,pvVar7);
    }
    if (DAT_0046c4e8 == (HWND)0x0) {
      uVar2 = 0;
    }
    else {
      if (DAT_0045f080 == 0) {
        UpdateWindow(DAT_0046c4e8);
      }
      uVar2 = 1;
    }
  }
  return uVar2;
}

