// rea decompile_all
// name: FUN_0042c57e
// address: 0x42c57e
// body: available
// range: 0x42c57e-0x42c899 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __cdecl FUN_0042c57e(int param_1,int param_2,int param_3)

{
  int *piVar1;
  ushort uVar2;
  int iVar3;
  ushort uVar4;
  ushort uVar5;
  ushort uVar6;
  byte local_54;
  int local_4c;
  int local_40;
  int local_3c;
  ushort *local_2c;
  byte local_24;
  byte local_20;
  ushort *local_18;
  int local_10;
  int local_8;
  
  local_24 = 0x1a - (char)_DAT_00485c78;
  local_54 = 0xf - (char)_DAT_00485c80;
  local_20 = 4 - (char)DAT_00485c8c;
  local_2c = (ushort *)(param_2 + 0x25940);
  local_8 = 0;
  local_18 = (ushort *)(param_1 + 0x25940);
  for (local_3c = 0; local_3c < 200; local_3c = local_3c + 1) {
    for (local_40 = 0; local_40 < 0x140; local_40 = local_40 + 1) {
      uVar6 = *local_2c;
      uVar4 = uVar6 & DAT_00485c7c;
      uVar5 = DAT_00485c86 & uVar6;
      uVar6 = DAT_00485c84 & uVar6;
      uVar2 = *local_18;
      *(uint *)(param_3 + local_8 * 4) =
           (uint)(DAT_00485c84 & uVar2) << (local_20 & 0x1f) |
           (uint)(DAT_00485c86 & uVar2) << (local_54 & 0x1f) |
           (uint)(uVar2 & DAT_00485c7c) << (local_24 & 0x1f);
      *(int *)(param_3 + 0x4b000 + local_8 * 4) =
           (int)(((uint)uVar6 << (local_20 & 0x1f) | (uint)uVar5 << (local_54 & 0x1f) |
                  (uint)uVar4 << (local_24 & 0x1f) | 0x2004000) - *(int *)(param_3 + local_8 * 4))
           >> 3;
      local_8 = local_8 + 1;
      local_18 = local_18 + 1;
      local_2c = local_2c + 1;
    }
    local_18 = local_18 + 0x140;
    local_2c = local_2c + 0x140;
  }
  for (local_4c = 0; local_4c < 8; local_4c = local_4c + 1) {
    local_8 = 0;
    local_10 = 0;
    for (local_40 = 0; local_40 < 200; local_40 = local_40 + 1) {
      for (local_3c = 0; local_3c < 0x140; local_3c = local_3c + 1) {
        piVar1 = (int *)(param_3 + local_8 * 4);
        *piVar1 = *piVar1 + *(int *)(param_3 + 0x4b000 + local_8 * 4);
        iVar3 = *(int *)(param_3 + local_8 * 4);
        ((ushort *)(param_1 + 0x25940))[local_10] =
             (ushort)(iVar3 >> (local_20 & 0x1f)) & DAT_00485c84 |
             (ushort)(iVar3 >> (local_54 & 0x1f)) & DAT_00485c86 |
             (ushort)(iVar3 >> (local_24 & 0x1f)) & DAT_00485c7c;
        local_8 = local_8 + 1;
        local_10 = local_10 + 1;
      }
      local_10 = local_10 + 0x140;
    }
    FUN_0041842b(param_1,0x3e800);
  }
  return;
}

