// rea decompile_all
// name: FUN_00412aea
// address: 0x412aea
// body: available
// range: 0x412aea-0x412cc3 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


undefined2 __cdecl FUN_00412aea(int param_1,byte param_2,FILE *param_3)

{
  byte local_28 [4];
  byte local_24 [4];
  byte local_20;
  byte local_1c;
  byte local_18;
  char *local_14;
  byte local_10;
  char *local_c;
  undefined2 local_8;
  
  local_8 = 0;
  local_18 = 0;
  local_10 = 0;
  while (local_10 < param_2) {
    *(undefined4 *)(param_1 + (uint)local_18 * 4) = 0;
    local_c = (char *)0x0;
    local_1c = 0;
    __fread_lk((char *)local_28,1,1,param_3);
    for (; local_1c < local_28[0]; local_1c = local_1c + 1) {
      if (local_1c == 0) {
        FUN_00412cc4((int *)((uint)local_18 * 4 + param_1));
        local_c = *(char **)(param_1 + (uint)local_18 * 4);
      }
      else {
        FUN_00412cc4((int *)(local_c + 8));
        local_c = *(char **)(local_c + 8);
      }
      __fread_lk(local_c,2,1,param_3);
      __fread_lk(local_c + 2,2,1,param_3);
      FUN_00412762((undefined4 *)(local_c + 4),param_3);
      __fread_lk((char *)local_24,1,1,param_3);
      local_14 = (char *)0x0;
      for (local_20 = 0; local_20 < local_24[0]; local_20 = local_20 + 1) {
        if (local_20 == 0) {
          FUN_00412cc4((int *)(local_c + 0xc));
          local_14 = *(char **)(local_c + 0xc);
        }
        else {
          FUN_00412cc4((int *)(local_14 + 0xc));
          local_14 = *(char **)(local_14 + 0xc);
        }
        __fread_lk(local_14,2,1,param_3);
        __fread_lk(local_14 + 2,2,1,param_3);
        FUN_00412762((undefined4 *)(local_14 + 4),param_3);
      }
    }
    if (local_28[0] != 0) {
      local_10 = local_10 + 1;
    }
    local_18 = local_18 + 1;
  }
  return local_8;
}

