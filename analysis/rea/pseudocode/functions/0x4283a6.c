// rea decompile_all
// name: FUN_004283a6
// address: 0x4283a6
// body: available
// range: 0x4283a6-0x42875c inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


undefined4 __cdecl FUN_004283a6(int *param_1,uint *param_2)

{
  undefined4 uVar1;
  MMRESULT MVar2;
  undefined4 *puVar3;
  undefined4 local_154;
  undefined4 local_150;
  DWORD local_14c;
  undefined2 *local_144;
  uint local_140;
  int *local_13c;
  undefined4 local_138;
  char *local_134;
  int local_130;
  undefined2 local_12c;
  undefined2 local_12a;
  int local_128;
  int local_124;
  undefined2 local_120;
  undefined2 local_11e;
  undefined2 local_11c;
  FILE *local_118;
  undefined4 local_114;
  undefined4 local_110;
  uint local_10c [65];
  DWORD local_8;
  
  if (param_1 == (int *)0x0) {
    uVar1 = 0;
  }
  else if (DAT_00461934 == (int *)0x0) {
    uVar1 = 0;
  }
  else if ((param_2 == (uint *)0x0) || ((char)*param_2 == '\0')) {
    uVar1 = 0;
  }
  else {
    FUN_0042793f(*param_1);
    if ((*(int *)(&DAT_00485c90 + *param_1 * 4) == 0) ||
       (*(char *)(*(int *)(&DAT_00485c90 + *param_1 * 4) + 5) == '\0')) {
      FUN_00428f8c(local_10c,param_2,(uint *)&DAT_004601ec);
      local_118 = (FILE *)FUN_0044bc20((char *)local_10c,&DAT_004601f4);
      if (local_118 == (FILE *)0x0) {
        FUN_00428f8c(local_10c,param_2,(uint *)&DAT_004601f8);
        local_118 = (FILE *)FUN_0044bc20((char *)local_10c,&DAT_00460200);
        if (local_118 == (FILE *)0x0) {
          return 0;
        }
        FUN_0044c250(local_118,0,2);
        local_8 = FUN_0044c370(&local_118->_ptr);
        FUN_0044c250(local_118,0,0);
        _memset(&local_154,0,0x14);
        local_154 = 0x14;
        local_150 = 0xe0;
        local_14c = local_8;
        local_144 = &local_12c;
        local_12c = 1;
        local_11c = 0;
        if (param_1[3] == 1) {
          local_12a = 2;
        }
        else {
          local_12a = 1;
        }
        local_128 = param_1[4];
        if (param_1[2] == 0) {
          local_11e = 8;
          local_120 = 1;
          local_124 = param_1[4];
        }
        else {
          local_11e = 0x10;
          if (param_1[3] == 0) {
            local_120 = 2;
            local_124 = param_1[4] * 2;
          }
          else {
            local_120 = 4;
            local_124 = param_1[4] << 2;
          }
        }
        local_130 = (**(code **)(*DAT_00461934 + 0xc))(DAT_00461934,&local_154,&local_13c,0);
        if (local_130 != 0) {
          return 0;
        }
        local_130 = (**(code **)(*local_13c + 0x2c))
                              (local_13c,0,local_8,&local_134,&local_110,&local_138,&local_114,0);
        if (local_130 != 0) {
          return 0;
        }
        local_140 = __fread_lk(local_134,1,local_8,local_118);
        _fclose(local_118);
        local_130 = (**(code **)(*local_13c + 0x4c))
                              (local_13c,local_134,local_110,local_138,local_114);
        if (local_130 != 0) {
          return 0;
        }
      }
      else {
        _fclose(local_118);
        MVar2 = FUN_004058fd((LPSTR)local_10c,&local_8,&local_13c);
        if (MVar2 != 0) {
          return 0;
        }
      }
      puVar3 = FUN_00428229(local_13c,local_8);
      *(undefined4 **)(&DAT_00485c90 + *param_1 * 4) = puVar3;
      uVar1 = 1;
    }
    else {
      uVar1 = 1;
    }
  }
  return uVar1;
}

