// rea decompile_all
// name: FUN_0040f135
// address: 0x40f135
// body: available
// range: 0x40f135-0x40f341 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


ushort * __cdecl FUN_0040f135(char *param_1,int *param_2,uint *param_3,ushort *param_4,int param_5)

{
  ushort *local_244;
  undefined4 local_240 [33];
  int local_1bc;
  int local_1b8;
  undefined4 local_178;
  undefined1 local_174;
  char local_172;
  undefined4 local_170;
  undefined1 local_16c;
  undefined4 local_168;
  int local_160;
  uint local_15c;
  int local_154;
  uint local_144;
  int local_14;
  FILE *local_10;
  undefined4 *local_c;
  ushort *local_8;
  
  local_10 = (FILE *)FUN_0044bc20(param_1,&DAT_0045e7a4);
  if (local_10 == (FILE *)0x0) {
    local_244 = (ushort *)0x0;
  }
  else {
    local_1bc = FUN_0043e900(local_240);
    FUN_0043de70(&local_1bc,0x3d,0x1a8);
    FUN_0043e7b0((int)&local_1bc,local_10);
    FUN_0043dfc0(&local_1bc,'\x01');
    local_16c = 0;
    local_170 = 1;
    if (local_172 == '\0') {
      local_168 = 0xd8;
    }
    local_178 = 1;
    local_174 = 0;
    FUN_0043e3d0(&local_1bc);
    local_14 = local_154 * local_160;
    local_c = (undefined4 *)(**(code **)(local_1b8 + 8))(&local_1bc,1,local_14,1);
    if (param_4 == (ushort *)0x0) {
      local_244 = (ushort *)FUN_0044c2f0(local_160 * local_15c * 2);
      param_5 = local_160;
      local_8 = local_244;
    }
    else {
      local_244 = param_4;
      local_8 = param_4;
      if ((local_160 != *param_2) || (local_15c != *param_3)) {
        return (ushort *)0x0;
      }
    }
    while (local_144 < local_15c) {
      FUN_0043e560(&local_1bc,local_c,1);
      FUN_0040f080((int *)*local_c,local_160,local_8);
      local_8 = local_8 + param_5;
    }
    FUN_0043e320(&local_1bc);
    FUN_0043df40((int)&local_1bc);
    _fclose(local_10);
    *param_2 = local_160;
    *param_3 = local_15c;
  }
  return local_244;
}

