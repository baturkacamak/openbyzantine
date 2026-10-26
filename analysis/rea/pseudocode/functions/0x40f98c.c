// rea decompile_all
// name: FUN_0040f98c
// address: 0x40f98c
// body: available
// range: 0x40f98c-0x40fbf5 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_0040f98c(int param_1)

{
  int local_8;
  
  if (*(int *)(&DAT_00482ad0 + param_1 * 4) != 0) {
    FUN_00413802(*(LPVOID *)(*(int *)(&DAT_00482ad0 + param_1 * 4) + 4));
    *(undefined4 *)(*(int *)(&DAT_00482ad0 + param_1 * 4) + 4) = 0;
    FUN_00413802((LPVOID)**(undefined4 **)(&DAT_00482ad0 + param_1 * 4));
    **(undefined4 **)(&DAT_00482ad0 + param_1 * 4) = 0;
    FUN_00413729(*(int *)(&DAT_00482ad0 + param_1 * 4) + 0x4c4);
    FUN_00413729(*(int *)(&DAT_00482ad0 + param_1 * 4) + 0x58c);
    FUN_0041383a(*(int *)(&DAT_00482ad0 + param_1 * 4) + 0x654);
    for (local_8 = 0; local_8 < 0x32; local_8 = local_8 + 1) {
      if (*(int *)(*(int *)(&DAT_00482ad0 + param_1 * 4) + 0xa8 + local_8 * 4) != 0) {
        FUN_0044ac40(*(LPVOID *)(*(int *)(&DAT_00482ad0 + param_1 * 4) + 0xa8 + local_8 * 4));
      }
      if (*(int *)(*(int *)(&DAT_00482ad0 + param_1 * 4) + 0x170 + local_8 * 4) != 0) {
        FUN_0044ac40(*(LPVOID *)(*(int *)(&DAT_00482ad0 + param_1 * 4) + 0x170 + local_8 * 4));
      }
      if (*(int *)(*(int *)(&DAT_00482ad0 + param_1 * 4) + 0x238 + local_8 * 4) != 0) {
        FUN_0044ac40(*(LPVOID *)(*(int *)(&DAT_00482ad0 + param_1 * 4) + 0x238 + local_8 * 4));
      }
    }
    for (local_8 = 0;
        local_8 < (int)(uint)*(ushort *)(*(int *)(&DAT_00482ad0 + param_1 * 4) + 0x71c);
        local_8 = local_8 + 1) {
      if (*(int *)(*(int *)(&DAT_00482ad0 + param_1 * 4) + 0x720 + local_8 * 4) != 0) {
        FUN_0044ac40(*(LPVOID *)(*(int *)(&DAT_00482ad0 + param_1 * 4) + 0x720 + local_8 * 4));
      }
    }
    for (local_8 = 0;
        local_8 < (int)(uint)*(ushort *)(*(int *)(&DAT_00482ad0 + param_1 * 4) + 0x12d8);
        local_8 = local_8 + 1) {
      if (*(int *)(*(int *)(&DAT_00482ad0 + param_1 * 4) + 0x12dc + local_8 * 4) != 0) {
        FUN_0044ac40(*(LPVOID *)(*(int *)(&DAT_00482ad0 + param_1 * 4) + 0x12dc + local_8 * 4));
      }
    }
    FUN_0044ac40(*(LPVOID *)(&DAT_00482ad0 + param_1 * 4));
    *(undefined4 *)(&DAT_00482ad0 + param_1 * 4) = 0;
  }
  return;
}

