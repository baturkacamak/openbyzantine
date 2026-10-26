// rea decompile_all
// name: FUN_00426261
// address: 0x426261
// body: available
// range: 0x426261-0x42658e inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


int __cdecl FUN_00426261(uint param_1,FILE *param_2)

{
  int iVar1;
  uint local_28 [4];
  int local_18;
  uint local_14;
  int local_10;
  undefined1 local_c;
  int local_8;
  
  local_8 = 0;
  iVar1 = _fgetc(param_2);
  if ((char)iVar1 == '\0') {
    local_18 = 0;
    local_c = 0;
    do {
      iVar1 = _fgetc(param_2);
      *(char *)((int)local_28 + local_18) = (char)iVar1;
      iVar1 = local_18;
      local_18 = local_18 + 1;
    } while (*(char *)((int)local_28 + iVar1) != '\0');
    iVar1 = FUN_00410669(param_1,&local_14,local_28);
    local_8 = local_8 + iVar1;
    if (local_8 == 0) {
      local_10 = *(int *)(&DAT_00482ad0 + local_14 * 4);
      __fread_lk((char *)(local_10 + 8),2,1,param_2);
      iVar1 = _fgetc(param_2);
      *(char *)(local_10 + 10) = (char)iVar1;
      iVar1 = _fgetc(param_2);
      *(char *)(local_10 + 0xb) = (char)iVar1;
      iVar1 = _fgetc(param_2);
      *(char *)(local_10 + 0xc) = (char)iVar1;
      iVar1 = _fgetc(param_2);
      *(char *)(local_10 + 0xd) = (char)iVar1;
      iVar1 = _fgetc(param_2);
      *(char *)(local_10 + 0xe) = (char)iVar1;
      __fread_lk((char *)(local_10 + 0xf),1,0x32,param_2);
      __fread_lk((char *)(local_10 + 0x41),1,0x32,param_2);
      __fread_lk((char *)(local_10 + 0x73),1,0x32,param_2);
      for (local_18 = 0; local_18 < 0x32; local_18 = local_18 + 1) {
        iVar1 = FUN_0042658f(*(int *)(local_10 + 0x4c4 + local_18 * 4),param_2);
        local_8 = local_8 + iVar1;
      }
      for (local_18 = 0; local_18 < 0x32; local_18 = local_18 + 1) {
        iVar1 = FUN_0042658f(*(int *)(local_10 + 0x58c + local_18 * 4),param_2);
        local_8 = local_8 + iVar1;
      }
      for (local_18 = 0; local_18 < 0x32; local_18 = local_18 + 1) {
        iVar1 = FUN_00426662(local_10,(int *)(local_18 * 4 + local_10 + 0xa8),param_2);
        local_8 = local_8 + iVar1;
      }
      for (local_18 = 0; local_18 < 0x32; local_18 = local_18 + 1) {
        iVar1 = FUN_00426662(local_10,(int *)(local_18 * 4 + local_10 + 0x170),param_2);
        local_8 = local_8 + iVar1;
      }
      for (local_18 = 0; local_18 < 0x32; local_18 = local_18 + 1) {
        iVar1 = FUN_00426662(local_10,(int *)(local_18 * 4 + local_10 + 0x238),param_2);
        local_8 = local_8 + iVar1;
      }
      for (local_18 = 0; local_18 < 0x32; local_18 = local_18 + 1) {
        iVar1 = FUN_00426788(local_10,(int *)(local_18 * 4 + local_10 + 0x3c8),param_2,'\0');
        local_8 = local_8 + iVar1;
      }
      __fread_lk((char *)(local_10 + 0x490),1,0x32,param_2);
      for (local_18 = 0; local_18 < 0x32; local_18 = local_18 + 1) {
        FUN_00426892(local_10,(int *)(local_18 * 4 + local_10 + 0x654),param_2);
      }
      FUN_0044ba00((uint *)(local_10 + 0x1e94),local_28);
    }
  }
  return local_8;
}

