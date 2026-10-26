// rea decompile_all
// name: FUN_00404b97
// address: 0x404b97
// body: available
// range: 0x404b97-0x404ce9 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_00404b97(uint *param_1,short param_2,uint *param_3,int param_4)

{
  uint local_804 [512];
  
  _memset(local_804,0,0x800);
  _memset(&DAT_0045c120,0,0x100);
  FUN_0044ba00((uint *)&DAT_0045c120,param_1);
  if (param_4 != -0x29a) {
    _sprintf((char *)local_804,&DAT_0045d094,param_4);
    FUN_0044ba08((uint *)&DAT_0045c120,local_804);
  }
  _sprintf((char *)local_804,s____d__0045d09c,(int)param_2);
  FUN_0044ba08((uint *)&DAT_0045c120,local_804);
  FUN_0044ba08((uint *)&DAT_0045c120,param_3);
  _memset(local_804,0,0x800);
  FUN_0044ba00(local_804,param_1);
  FUN_0044ba08(local_804,(uint *)&DAT_0045d0a4);
  if (param_3 != (uint *)0x0) {
    if (param_4 == 0x29a) {
      FUN_00404a15(local_804,param_2,param_3,-0x29a);
    }
    else {
      FUN_00404a15(local_804,param_2,param_3,param_4);
    }
  }
  return;
}

