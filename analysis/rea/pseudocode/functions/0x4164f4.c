// rea decompile_all
// name: FUN_004164f4
// address: 0x4164f4
// body: available
// range: 0x4164f4-0x41670f inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


undefined4 __cdecl FUN_004164f4(undefined4 *param_1)

{
  ushort uVar1;
  undefined4 uVar2;
  char local_548 [260];
  char local_444;
  char local_443;
  uint local_438;
  byte local_434;
  int local_42c;
  int local_424;
  FILE *local_420;
  undefined4 local_41c;
  SIZE_T local_418;
  char local_414 [4];
  ushort *local_410;
  
  local_420 = (FILE *)0x0;
  local_418 = 0;
  local_41c = 0;
  local_420 = (FILE *)FUN_0044bc20((char *)*param_1,&DAT_0045f1e8);
  __fread_lk(&local_444,1,0x12,local_420);
  local_418 = ((int)(uint)local_434 >> 3) * (local_438 & 0xffff) * (local_438 >> 0x10);
  if ((local_443 == '\0') && (local_444 == '\0')) {
    uVar2 = FUN_0044c2f0(local_418);
    param_1[3] = uVar2;
    local_410 = (ushort *)(((local_438 >> 0x10) - 1) * (local_438 & 0xffff) * 2 + param_1[3]);
    for (local_424 = 0; local_424 < (int)(local_438 >> 0x10); local_424 = local_424 + 1) {
      for (local_42c = 0; local_42c < (int)(local_438 & 0xffff); local_42c = local_42c + 1) {
        __fread_lk(local_414,2,1,local_420);
        uVar1 = FUN_004164a0(local_414._0_2_);
        *local_410 = uVar1;
        local_410 = local_410 + 1;
      }
      local_410 = local_410 + (local_438 & 0xffff) * -2;
    }
  }
  _fclose(local_420);
  param_1[6] = (uint)local_434;
  param_1[5] = local_438 >> 0x10;
  param_1[4] = local_438 & 0xffff;
  if (param_1[4] == 0) {
    _sprintf(local_548,(char *)*param_1);
  }
  return 1;
}

