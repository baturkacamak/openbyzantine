// rea decompile_all
// name: FUN_00402a44
// address: 0x402a44
// body: available
// range: 0x402a44-0x402ba1 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void FUN_00402a44(void)

{
  int local_8c;
  undefined4 local_88 [9];
  int local_64;
  char *local_1c;
  int local_18;
  int local_14;
  int local_10;
  int local_c;
  int local_8;
  
  local_c = 0;
  local_14 = 0;
  if (DAT_0045c080 == 0) {
    _memset(local_88,0,0x6c);
    local_88[0] = 0x6c;
    DAT_0045c080 = 1;
    local_1c = s__columbus_art_endgame_a00_tga_0045c0fc;
    FUN_00416710(s__columbus_art_endgame_a00_tga_0045c0fc,&DAT_0045c078,1,(uint *)&DAT_00468c9c,
                 (uint *)&DAT_00468b94);
    FUN_00416710(local_1c,&DAT_0045c07c,0,(uint *)0x0,(uint *)0x0);
    (**(code **)(*DAT_0045c07c + 100))(DAT_0045c07c,0,local_88,1,0);
    local_8c = local_64;
    local_8 = (DAT_00468ca0 * local_14 + local_c) * 2 + DAT_0045c040;
    for (local_10 = 0; local_10 < DAT_00468b94; local_10 = local_10 + 1) {
      for (local_18 = 0; local_18 < DAT_00468c9c; local_18 = local_18 + 1) {
        *(undefined2 *)(local_8c + local_18 * 2) = *(undefined2 *)(local_8 + local_18 * 2);
      }
      local_8 = local_8 + DAT_00468ca0 * 2;
      local_8c = local_8c + DAT_00468c9c * 2;
    }
    (**(code **)(*DAT_0045c07c + 0x80))(DAT_0045c07c,0);
  }
  return;
}

