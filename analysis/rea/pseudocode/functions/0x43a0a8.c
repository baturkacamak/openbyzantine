// rea decompile_all
// name: FUN_0043a0a8
// address: 0x43a0a8
// body: available
// range: 0x43a0a8-0x43a20c inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


undefined4 __fastcall FUN_0043a0a8(undefined4 *param_1)

{
  int iVar1;
  undefined4 uVar2;
  longlong lVar3;
  undefined1 local_90 [32];
  undefined4 local_70;
  undefined4 local_6c;
  undefined4 local_60;
  
  iVar1 = AVIFileGetStream(param_1[5],param_1 + 0x18,0x73647561,0);
  if (iVar1 == 0) {
    iVar1 = AVIStreamInfoA(param_1[0x18],local_90,0x8c);
    if (iVar1 == 0) {
      param_1[0x1c] = local_6c;
      if (param_1[0x1c] == 0) {
        MessageBoxA((HWND)*param_1,s_No_Audio_Skew_on_File_00461828,(LPCSTR)0x0,0);
        uVar2 = 0xffffffff;
      }
      else {
        param_1[0x1a] = local_60;
        param_1[0x1b] = local_70;
        if (DAT_00462ae8 != 0) {
          __adj_fdiv_m64(0,0x40d58880);
        }
        lVar3 = __ftol();
        param_1[0x19] = (int)lVar3;
        uVar2 = 0;
      }
    }
    else {
      FUN_0043a20d((int)param_1);
      param_1[0x1c] = 0;
      uVar2 = 0xffffffff;
    }
  }
  else {
    MessageBoxA((HWND)*param_1,s_Video_w_out_audio_00461814,(LPCSTR)0x0,0);
    param_1[0x1c] = 0;
    uVar2 = 0xffffffff;
  }
  return uVar2;
}

