// rea decompile_all
// name: FUN_0040d031
// address: 0x40d031
// body: available
// range: 0x40d031-0x40d171 inclusive
// range: 0x40d177-0x40d190 inclusive
// range: 0x40d1a9-0x40d2a0 inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_0040d031(uint *param_1,int param_2,int param_3,int param_4)

{
  int iVar1;
  FILE *local_114;
  uint *local_110;
  uint local_10c [65];
  int local_8;
  
  local_8 = 1;
  if (((param_2 != 2) || (iVar1 = FUN_0041d440(5), iVar1 != 0)) &&
     ((param_2 != 3 || (iVar1 = FUN_0041d440(4), iVar1 != 0)))) {
    switch(param_2) {
    case 0:
      local_110 = (uint *)s_video_slide_0045db4c;
      DAT_00460664 = 0x65;
      break;
    case 1:
      local_110 = (uint *)s_video_intro_0045db28;
      local_8 = 0;
      break;
    case 2:
      DAT_00460664 = 0x6a;
      local_110 = (uint *)s_video_site_0045db34;
      break;
    case 3:
      local_110 = (uint *)s_video_transit_0045db58;
      if (DAT_00464878 == 0) {
        if (DAT_00460668 == 2) {
          DAT_00460664 = 100;
          local_8 = 0;
        }
        else {
          DAT_00460664 = 0x65;
        }
      }
      else {
        DAT_00460664 = 100;
      }
      break;
    case 4:
      DAT_00460664 = 0x67;
      local_110 = (uint *)s_video_char_0045db40;
      break;
    case 5:
      local_110 = (uint *)s_video_anim_0045db68;
      local_8 = 0;
      break;
    default:
      local_110 = (uint *)s_video_0045db74;
    }
    _sprintf((char *)local_10c,s_avi__s_avi_0045db7c,param_1);
    local_114 = (FILE *)FUN_0044bc20((char *)local_10c,&DAT_0045db88);
    if (local_114 == (FILE *)0x0) {
      FUN_0040c624(local_10c,local_110,param_1,(uint *)&DAT_0045db8c);
      local_114 = (FILE *)FUN_0044bc20((char *)local_10c,&DAT_0045db94);
      if (local_114 == (FILE *)0x0) {
        return;
      }
    }
    _fclose(local_114);
    FUN_0040d3af(local_10c,param_2,param_3,param_4,local_8,1);
    iVar1 = __strcmpi((char *)param_1,s_mcaft1a1_0045db98);
    if (iVar1 == 0) {
      FUN_0040da1f();
    }
    if (param_2 == 5) {
      FUN_0040da1f();
    }
    else {
      DAT_004859cd = 1;
    }
  }
  return;
}

