// rea decompile_all
// name: FUN_00428f8c
// address: 0x428f8c
// body: available
// range: 0x428f8c-0x429048 inclusive
// range: 0x42905d-0x429079 inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_00428f8c(uint *param_1,uint *param_2,uint *param_3)

{
  FILE *_File;
  undefined4 uVar1;
  uint *local_c;
  
  _sprintf((char *)param_1,s_sounds__s_s_00460218,param_2,param_3);
  _File = (FILE *)FUN_0044bc20((char *)param_1,&DAT_00460224);
  if (_File == (FILE *)0x0) {
    uVar1 = FUN_00438afa();
    switch(uVar1) {
    case 1:
      local_c = (uint *)s_sounds_vt_00460258;
      break;
    case 2:
      local_c = (uint *)s_sounds_vb_00460228;
      break;
    case 3:
      local_c = (uint *)s_sounds_vc_00460240;
      break;
    case 4:
      local_c = (uint *)s_sounds_vh_0046024c;
      break;
    case 5:
      local_c = (uint *)s_sounds_va_00460234;
      break;
    default:
      local_c = (uint *)s_sounds_00460264;
    }
    FUN_0040c624(param_1,local_c,param_2,param_3);
  }
  else {
    _fclose(_File);
  }
  return;
}

