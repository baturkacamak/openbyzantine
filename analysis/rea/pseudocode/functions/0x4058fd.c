// rea decompile_all
// name: FUN_004058fd
// address: 0x4058fd
// body: available
// range: 0x4058fd-0x405a95 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


MMRESULT __cdecl FUN_004058fd(LPSTR param_1,uint *param_2,undefined4 *param_3)

{
  int iVar1;
  _MMCKINFO local_68;
  undefined4 local_54;
  undefined4 local_50;
  uint local_4c;
  HGLOBAL local_44;
  uint local_40;
  int *local_3c;
  HGLOBAL local_38;
  undefined4 local_34;
  int local_30;
  undefined4 local_2c;
  undefined4 local_28;
  uint local_24;
  HMMIO local_20;
  _MMCKINFO local_1c;
  MMRESULT local_8;
  
  local_3c = (int *)0x0;
  *param_3 = 0;
  *param_2 = 0;
  local_8 = FUN_00404d10(param_1,&local_20,(int *)&local_38,&local_1c);
  if ((local_8 == 0) && (local_8 = FUN_00404f64(&local_20,&local_68,&local_1c), local_8 == 0)) {
    local_24 = local_68.cksize;
    _memset(&local_54,0,0x14);
    local_54 = 0x14;
    local_50 = 0xe0;
    local_4c = local_24;
    local_44 = local_38;
    iVar1 = (**(code **)(*DAT_00461934 + 0xc))(DAT_00461934,&local_54,param_3,0);
    if (iVar1 == 0) {
      local_3c = (int *)*param_3;
      (**(code **)(*local_3c + 0x2c))(local_3c,0,local_24,&local_30,&local_28,&local_34,&local_2c,0)
      ;
      local_8 = FUN_00404fe3(local_20,local_68.cksize,local_30,(int)&local_68,&local_40);
      (**(code **)(*local_3c + 0x4c))(local_3c,local_30,local_28,local_34,local_2c);
      if (local_8 == 0) {
        *param_2 = local_40;
        goto LAB_00405a5f;
      }
    }
  }
  if (local_3c != (int *)0x0) {
    (**(code **)(*local_3c + 8))(local_3c);
  }
LAB_00405a5f:
  if (local_38 != (HGLOBAL)0x0) {
    GlobalFree(local_38);
  }
  if (local_20 != (HMMIO)0x0) {
    mmioClose(local_20,0);
  }
  return local_8;
}

