// rea decompile_all
// name: FUN_00416b4b
// address: 0x416b4b
// body: available
// range: 0x416b4b-0x416d04 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


undefined4 __cdecl FUN_00416b4b(uint *param_1,int param_2,int param_3,int *param_4)

{
  ushort uVar1;
  undefined4 uVar2;
  char local_7c [2];
  undefined1 local_7a;
  undefined4 local_70;
  undefined1 local_6c;
  undefined1 local_6b;
  uint local_68 [20];
  int local_18;
  FILE *local_14;
  char *local_10;
  LPVOID local_c;
  
  _memset(local_7c,0,0x12);
  local_70 = CONCAT22((short)param_3,0x280);
  local_6c = 0x10;
  local_6b = 1;
  local_7a = 2;
  if (param_4 != (int *)0x0) {
    *param_4 = -1;
  }
  if (param_1 == (uint *)0x0) {
    DAT_0045f1e4 = DAT_0045f1e4 + 1;
    _sprintf((char *)local_68,s_screen_02d_tga_0045f1f0,DAT_0045f1e4);
  }
  else {
    FUN_0044ba00(local_68,param_1);
  }
  local_14 = (FILE *)FUN_0044bc20((char *)local_68,&DAT_0045f200);
  if (local_14 == (FILE *)0x0) {
    uVar2 = 1;
  }
  else {
    FUN_0044a460(local_7c,1,0x12,local_14);
    local_c = (LPVOID)FUN_0044c2f0(param_3 * 0x500);
    for (local_18 = 0; local_18 < param_3 * 0x280; local_18 = local_18 + 1) {
      uVar1 = FUN_00416ad8(*(ushort *)(param_2 + local_18 * 2));
      *(ushort *)((int)local_c + local_18 * 2) = uVar1;
    }
    local_10 = (char *)(((local_70 >> 0x10) - 1) * (local_70 & 0xffff) * 2 + (int)local_c);
    for (local_18 = 0; local_18 < (int)(local_70 >> 0x10); local_18 = local_18 + 1) {
      FUN_0044a460(local_10,2,local_70 & 0xffff,local_14);
      local_10 = local_10 + (local_70 & 0xffff) * -2;
    }
    _fclose(local_14);
    FUN_0044ac40(local_c);
    if (param_4 != (int *)0x0) {
      *param_4 = DAT_0045f1e4;
    }
    uVar2 = 0;
  }
  return uVar2;
}

