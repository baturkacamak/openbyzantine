// rea decompile_all
// name: FUN_0043c704
// address: 0x43c704
// body: available
// range: 0x43c704-0x43c8d1 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


uint __cdecl FUN_0043c704(undefined4 param_1,undefined4 param_2,int param_3,uint *param_4)

{
  uint uVar1;
  char *pcVar2;
  byte local_10;
  byte local_c;
  undefined1 local_8;
  
  FUN_00437c8c();
  *(undefined1 *)param_4 = 0;
  local_10 = 0;
  for (local_c = 0; (int)(uint)local_c < param_3; local_c = local_c + 1) {
    FUN_004599c0(&DAT_00461880);
  }
  do {
    FUN_00437c8c();
    uVar1 = FUN_0041d3d5();
    local_8 = (undefined1)uVar1;
    if ((((uVar1 < 0x61) || (0x7a < uVar1)) && ((uVar1 < 0x41 || (0x5a < uVar1)))) &&
       (((((uVar1 < 0x30 || (0x39 < uVar1)) && (uVar1 != 0x2e)) &&
         ((uVar1 != 0x24 && (uVar1 != 0x27)))) && (uVar1 != 0x3d)))) {
      if ((uVar1 == 8) && (local_10 != 0)) {
        local_10 = local_10 - 1;
        *(undefined1 *)((uint)local_10 + (int)param_4) = 0;
      }
    }
    else if ((int)(uint)local_10 < param_3) {
      *(undefined1 *)((uint)local_10 + (int)param_4) = local_8;
      local_10 = local_10 + 1;
    }
    FUN_00437c8c();
    *(undefined1 *)((uint)local_10 + (int)param_4) = 0;
    FUN_004599c0(&DAT_00461884);
    pcVar2 = FUN_0044c580(param_4);
    for (local_c = (byte)pcVar2; (int)(uint)local_c < param_3; local_c = local_c + 1) {
      FUN_004599c0(&DAT_00461888);
    }
    FUN_00437c8c();
  } while ((uVar1 != 0xd) && (uVar1 != 0x1b));
  if (uVar1 == 0xd) {
    *(undefined1 *)(param_3 + (int)param_4) = 0;
  }
  else {
    *(undefined1 *)param_4 = 0;
    param_3 = (int)param_4;
  }
  return CONCAT31((int3)((uint)param_3 >> 8),local_8);
}

