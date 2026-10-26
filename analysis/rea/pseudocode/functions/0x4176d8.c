// rea decompile_all
// name: FUN_004176d8
// address: 0x4176d8
// body: available
// range: 0x4176d8-0x4177c6 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_004176d8(float param_1,float param_2,int *param_3)

{
  int iVar1;
  undefined4 local_24;
  undefined4 local_20;
  undefined4 local_1c;
  undefined4 local_18;
  undefined4 local_14;
  undefined4 local_10;
  undefined4 local_c;
  undefined4 local_8;
  
  local_14 = 0;
  local_10 = 0;
  local_c = 0x280;
  local_8 = 400;
  if ((DAT_0045f218 != 0) && (FUN_00417491(param_1,param_2,param_3), DAT_00486888 == param_3)) {
    if ((DAT_004603d4 != -1) &&
       (iVar1 = FUN_004177c7(*(char **)(&DAT_00483640 + DAT_004603d4 * 4)), iVar1 != 0)) {
      FUN_004177c7((char *)0x0);
    }
    if ((DAT_0045f214 != 0) && (DAT_0045c060 == 0)) {
      local_24 = 0;
      local_20 = 0;
      local_1c = 0x280;
      local_18 = 400;
      (**(code **)(*param_3 + 0x1c))(param_3,0,0x14,DAT_0045f218,&local_24,1);
    }
    FUN_00417054();
  }
  return;
}

