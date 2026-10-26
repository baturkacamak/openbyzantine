// rea decompile_all
// name: FUN_00410078
// address: 0x410078
// body: available
// range: 0x410078-0x4102ad inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

DWORD __cdecl FUN_00410078(char *param_1,int *param_2)

{
  short sVar1;
  DWORD DVar2;
  int iVar3;
  undefined4 uVar4;
  int *local_38;
  char local_30 [4];
  char local_2c [2];
  short local_2a;
  short local_28;
  short local_26;
  short local_24;
  uint local_16;
  int local_10;
  int local_c;
  FILE *local_8;
  
  local_8 = (FILE *)FUN_0044bc20(param_1,&DAT_0045e868);
  if (local_8 == (FILE *)0x0) {
    DVar2 = 0;
  }
  else {
    DVar2 = FUN_0041199e(local_8);
    sVar1 = FUN_0040cb26(local_8,local_30);
    local_c = (int)sVar1;
    for (local_10 = 0; local_10 < local_c; local_10 = local_10 + 1) {
      if (*param_2 == 0) {
        iVar3 = FUN_0044c2f0(0x40);
        *param_2 = iVar3;
        FUN_0040ffee((void *)*param_2,(short)local_c);
        *(char *)(*param_2 + 0x28) = local_30[0];
        local_38 = *(int **)(*param_2 + 0x18);
        __fread_lk(local_2c,0x1a,1,local_8);
        *(short *)*param_2 = *(short *)*param_2 + local_26;
        *(short *)(*param_2 + 2) = *(short *)(*param_2 + 2) + local_24;
      }
      else {
        uVar4 = FUN_0044c2f0(0x18);
        local_38[1] = uVar4;
        *(int **)(local_38[1] + 8) = local_38;
        local_38 = (int *)local_38[1];
        *(int **)(*(int *)(*param_2 + 0x18) + 8) = local_38;
        __fread_lk(local_2c,0x1a,1,local_8);
      }
      local_38[1] = *(undefined4 *)(*param_2 + 0x18);
      local_38[3] = (int)local_2a;
      local_38[4] = (int)local_28;
      local_38[5] = local_10;
      uVar4 = FUN_0044c2f0(local_16 * 2);
      *local_38 = uVar4;
      __fread_lk((char *)*local_38,local_16,2,local_8);
    }
    _fclose(local_8);
    if (_DAT_00485c78 != 10) {
      local_38 = *(int **)(*param_2 + 0x18);
      for (local_10 = 0; local_10 < local_c; local_10 = local_10 + 1) {
        FUN_0040be80(*local_38,local_38[4]);
        local_38 = (int *)local_38[1];
      }
    }
  }
  return DVar2;
}

