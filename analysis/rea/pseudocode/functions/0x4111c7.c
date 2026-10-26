// rea decompile_all
// name: FUN_004111c7
// address: 0x4111c7
// body: available
// range: 0x4111c7-0x41159e inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


int * __cdecl FUN_004111c7(uint *param_1)

{
  char *pcVar1;
  int *piVar2;
  int iVar3;
  uint *local_93c;
  void *local_938;
  int local_930;
  int *local_92c;
  int local_928;
  void *local_924;
  int *local_920;
  char *local_91c;
  int local_918;
  FILE *local_914;
  uint local_910 [65];
  uint *local_80c;
  char local_808 [2048];
  int local_8;
  
  local_938 = (void *)0x0;
  FUN_0040c624(local_910,(uint *)s_sscript_0045e9ec,param_1,(uint *)&DAT_0045e9e4);
  local_914 = (FILE *)FUN_0044bc20((char *)local_910,&DAT_0045e9f4);
  if (local_914 == (FILE *)0x0) {
    local_920 = (int *)0x0;
  }
  else {
    local_920 = (int *)FUN_0044c2f0(0x128);
    _memset(local_920,0,0x128);
    FUN_0044ce20(local_808,0x800,local_914);
    local_91c = _strtok(local_808,&DAT_0045e9f8);
    local_928 = 0;
    while (((local_928 < 5 && (local_91c != (char *)0x0)) &&
           (local_8 = _sscanf(local_91c,s__d__d_0045e9fc,&local_930,&local_918), local_8 == 2))) {
      local_920[local_928 + 6] = local_918 + local_930 + -100;
      iVar3 = FUN_00428b1e(local_930,local_918);
      local_920[local_928 + 1] = iVar3;
      local_91c = _strtok((char *)0x0,&DAT_0045ea04);
      local_928 = local_928 + 1;
    }
    *local_920 = local_928;
    FUN_0044ce20(local_808,0x800,local_914);
    while ((local_914->_flag & 0x10) == 0) {
      local_924 = (void *)FUN_0044c2f0(0x2c);
      _memset(local_924,0,0x2c);
      if (local_938 == (void *)0x0) {
        local_920[0x33] = (int)local_924;
      }
      else {
        *(void **)((int)local_938 + 0x28) = local_924;
      }
      local_93c = (uint *)_strtok(local_808,&DAT_0045ea08);
      if (local_93c != (uint *)0x0) {
        pcVar1 = FUN_0044c580(local_93c);
        local_91c = (char *)((int)local_93c + (int)pcVar1) + 1;
      }
      for (local_8 = 0; local_8 < *local_920; local_8 = local_8 + 1) {
        local_80c = (uint *)_strtok((char *)local_93c,&DAT_0045ea0c);
        local_92c = (int *)0x0;
        while (local_80c != (uint *)0x0) {
          piVar2 = (int *)FUN_0044c2f0(8);
          if (local_92c == (int *)0x0) {
            *(int **)((int)local_924 + local_8 * 4) = piVar2;
          }
          else {
            local_92c[1] = (int)piVar2;
          }
          iVar3 = FUN_004110e5((int)local_920,local_80c,local_8);
          *piVar2 = iVar3;
          piVar2[1] = 0;
          local_92c = piVar2;
          local_80c = (uint *)_strtok((char *)0x0,&DAT_0045ea10);
        }
        local_93c = (uint *)_strtok(local_91c,&DAT_0045ea14);
        if (local_93c != (uint *)0x0) {
          pcVar1 = FUN_0044c580(local_93c);
          local_91c = (char *)((int)local_93c + (int)pcVar1) + 1;
        }
      }
      local_938 = local_924;
      FUN_0044ce20(local_808,0x800,local_914);
    }
    _fclose(local_914);
  }
  return local_920;
}

