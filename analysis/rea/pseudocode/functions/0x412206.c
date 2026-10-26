// rea decompile_all
// name: FUN_00412206
// address: 0x412206
// body: available
// range: 0x412206-0x4123f8 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


int __cdecl FUN_00412206(uint *param_1,int param_2)

{
  int iVar1;
  uint uVar2;
  char *pcVar3;
  undefined4 uVar4;
  int local_11c;
  char *local_118;
  char *local_114;
  FILE *local_110;
  int local_10c;
  char local_108 [260];
  
  local_11c = -1;
  if (DAT_0045e7a8 == 0) {
    _sprintf(local_108,s_engine__s_0045ea7c,param_1);
    local_110 = (FILE *)FUN_0044bc20(local_108,&DAT_0045ea88);
  }
  else {
    local_110 = FUN_00411b4b(param_1,&local_11c);
  }
  if (local_110 == (FILE *)0x0) {
    local_10c = 0;
  }
  else {
    local_10c = 0;
    while (uVar2 = FUN_00411db1(&local_110->_ptr,local_11c), uVar2 == 0) {
      local_114 = (char *)0x0;
      do {
        local_118 = local_114 + 0x47fa70;
        local_118 = FUN_0044ce20(local_118,0x800 - (int)local_114,local_110);
        pcVar3 = FUN_0044c580((uint *)&DAT_0047fa70);
        local_114 = pcVar3;
        if (pcVar3[0x47fa6e] == '\r') {
          pcVar3[0x47fa6e] = '\n';
          local_114 = pcVar3 + -1;
          pcVar3[0x47fa6f] = '\0';
        }
      } while ((local_118 != (char *)0x0) && (*local_118 != '$'));
      if (local_118 == (char *)0x0) break;
      *local_118 = '\0';
      uVar4 = FUN_0044c2f0((SIZE_T)local_114);
      *(undefined4 *)(param_2 + local_10c * 4) = uVar4;
      iVar1 = local_10c * 4;
      local_10c = local_10c + 1;
      FUN_0044ba00(*(uint **)(param_2 + iVar1),(uint *)&DAT_0047fa70);
    }
    _fclose(local_110);
  }
  return local_10c;
}

