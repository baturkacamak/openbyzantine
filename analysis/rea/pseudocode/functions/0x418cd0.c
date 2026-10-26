// rea decompile_all
// name: FUN_00418cd0
// address: 0x418cd0
// body: available
// range: 0x418cd0-0x418de8 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_00418cd0(ushort *param_1,ushort *param_2)

{
  undefined2 *_Src;
  undefined4 uVar1;
  int local_24;
  int local_20;
  int local_1c;
  ushort *local_18;
  ushort *local_14;
  undefined2 *local_10;
  byte local_c;
  
  local_c = 1;
  for (local_24 = 0; local_24 < 8; local_24 = local_24 + 1) {
    local_14 = param_1;
    local_18 = param_2;
    local_10 = (undefined2 *)(DAT_00477344 + 0x6400);
    for (local_1c = 0; _Src = local_10, local_1c < 200; local_1c = local_1c + 1) {
      for (local_20 = 0; local_20 < 0x140; local_20 = local_20 + 1) {
        uVar1 = FUN_004066c6((uint)*local_18,(uint)*local_14,(uint)local_c);
        *local_10 = (short)uVar1;
        local_10[1] = *local_10;
        local_10 = local_10 + 2;
        local_14 = local_14 + 2;
        local_18 = local_18 + 2;
      }
      local_14 = local_14 + 0x280;
      local_18 = local_18 + 0x280;
      FID_conflict__memcpy(local_10,_Src,0x500);
      local_10 = local_10 + 0x280;
    }
    FUN_0041842b(DAT_00477344,0x3e800);
    local_c = local_c * '\x02' | 1;
  }
  return;
}

