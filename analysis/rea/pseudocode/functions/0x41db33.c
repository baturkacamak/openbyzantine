// rea decompile_all
// name: FUN_0041db33
// address: 0x41db33
// body: available
// range: 0x41db33-0x41dcc8 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_0041db33(int param_1,int param_2,undefined4 *param_3)

{
  undefined4 uVar1;
  uint local_120 [65];
  int local_1c;
  int local_18;
  uint *local_14;
  uint *local_10;
  char *local_c;
  FILE *local_8;
  
  local_14 = (uint *)FUN_0044c2f0(0x800);
  local_10 = (uint *)0x0;
  if (DAT_0045e7a8 == 0) {
    _sprintf((char *)local_120,s_engine__s_byz_0045f928,(&DAT_00485a10)[param_1]);
    local_8 = (FILE *)FUN_0044bc20((char *)local_120,&DAT_0045f938);
  }
  else {
    _sprintf((char *)local_120,s__s_byz_0045f920,(&DAT_00485a10)[param_1]);
    local_8 = FUN_00411b4b(local_120,&local_18);
  }
  if (local_8 != (FILE *)0x0) {
    for (local_1c = 0; local_1c <= param_2; local_1c = local_1c + 1) {
      local_10 = (uint *)FUN_0044ce20((char *)local_14,0x800,local_8);
    }
    _fclose(local_8);
  }
  if (local_10 == (uint *)0x0) {
    *(undefined1 *)local_14 = 0;
    local_10 = local_14;
    local_c = (char *)0x0;
  }
  else {
    local_c = FUN_0044c580(local_14);
    if ((local_c + -1)[(int)local_14] == '\n') {
      local_c = local_c + -1;
      local_c[(int)local_14] = '\0';
    }
    if ((local_c + -1)[(int)local_14] == '\r') {
      local_c = local_c + -1;
      local_c[(int)local_14] = '\0';
    }
  }
  uVar1 = FUN_0044c2f0((SIZE_T)(local_c + 1));
  *param_3 = uVar1;
  FUN_0044ba00((uint *)*param_3,local_10);
  FUN_0044ac40(local_14);
  return;
}

