// rea decompile_all
// name: FUN_0040e40f
// address: 0x40e40f
// body: available
// range: 0x40e40f-0x40e44d inclusive
// range: 0x40e453-0x40e626 inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_0040e40f(int *param_1,undefined4 param_2,int param_3)

{
  int iVar1;
  tagPOINT local_838;
  int local_830;
  int local_82c;
  HWND local_828;
  uint local_824 [512];
  int local_24;
  int local_20;
  int local_1c;
  int local_18;
  int local_14;
  int local_10;
  int local_c;
  int local_8;
  
  local_1c = DAT_0046c448;
  local_24 = DAT_0046c44c;
  local_18 = DAT_0046c450;
  local_14 = DAT_0046c454;
  local_10 = DAT_0046c458;
  local_c = DAT_0046c45c;
  if (DAT_0046c448 < 0) {
    local_18 = DAT_0046c450 - DAT_0046c448;
    local_1c = 0;
  }
  else {
    local_20 = DAT_0046c458 + DAT_0046c448 + -0x280;
    if (0 < local_20) {
      local_10 = DAT_0046c458 - local_20;
    }
  }
  if (DAT_0046c44c < 0) {
    local_14 = DAT_0046c454 - DAT_0046c44c;
    local_24 = 0;
  }
  else {
    local_20 = DAT_0046c45c + DAT_0046c44c + -0x1e0;
    if (0 < local_20) {
      local_c = DAT_0046c45c - local_20;
    }
  }
  iVar1 = FUN_00416220();
  if (iVar1 == 0) {
    local_828 = (HWND)FUN_004161f6();
    local_838.x = local_1c;
    local_838.y = local_24;
    local_830 = 0;
    local_82c = 0;
    ClientToScreen(local_828,&local_838);
    local_830 = (local_10 - local_18) + local_838.x;
    local_82c = (local_c - local_14) + local_838.y;
    (**(code **)(*param_1 + 0x14))(param_1,&local_838,param_2,&local_18,0x1000000,0);
  }
  else {
    if (param_3 == 0) {
      local_8 = (**(code **)(*param_1 + 0x1c))(param_1,local_1c,local_24,param_2,&local_18,0x10);
    }
    else {
      local_8 = (**(code **)(*param_1 + 0x1c))(param_1,local_1c,local_24,param_2,&local_18,0x11);
    }
    if (local_8 != 0) {
      FUN_0040305a(local_8,(char *)local_824);
      FUN_00404a15((uint *)s_in_mouse_blit_0045e730,0x2a7,
                   (uint *)s_D__Spanish_Shellsrc_mouse_c_0045e714,-0x29a);
      FUN_00404a15(local_824,0x2a8,(uint *)s_D__Spanish_Shellsrc_mouse_c_0045e740,-0x29a);
    }
  }
  return;
}

