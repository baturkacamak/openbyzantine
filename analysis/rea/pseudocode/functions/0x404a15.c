// rea decompile_all
// name: FUN_00404a15
// address: 0x404a15
// body: available
// range: 0x404a15-0x404b96 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


undefined4 __cdecl FUN_00404a15(uint *param_1,short param_2,uint *param_3,int param_4)

{
  wchar_t local_820 [1024];
  char *local_20;
  FILE *local_1c;
  uint local_18 [5];
  
  FUN_0044ba00((uint *)local_820,param_1);
  if (DAT_0045c220 == 0) {
    local_20 = &DAT_0045d018;
  }
  else {
    local_20 = &DAT_0045d014;
  }
  local_1c = (FILE *)FUN_0044bc20("MSG.LOG",local_20);
  if (local_1c == (FILE *)0x0) {
    FUN_0044c000(1);
  }
  if (DAT_0045c220 != 0) {
    FID_conflict__fwprintf(local_1c,(wchar_t *)s_COLUMBUS_LOG_FILE_0045d01c);
    FID_conflict__fwprintf(local_1c,(wchar_t *)s__________________________________0045d030);
    DAT_0045c220 = 0;
  }
  if (param_2 != -0x29a) {
    _sprintf((char *)local_18,s_Line____d__0045d074,(int)param_2);
    FUN_0044ba08((uint *)local_820,local_18);
    FUN_0044ba08((uint *)local_820,param_3);
    FUN_0044ba08((uint *)local_820,(uint *)&DAT_0045d080);
  }
  if (param_4 != -0x29a) {
    _sprintf((char *)local_18,s_Data____d_0045d084,param_4);
    FUN_0044ba08((uint *)local_820,local_18);
  }
  FUN_0044ba08((uint *)local_820,(uint *)&DAT_0045d090);
  FID_conflict__fwprintf(local_1c,local_820);
  _fflush(local_1c);
  _fclose(local_1c);
  return 1;
}

