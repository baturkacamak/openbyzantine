// rea decompile_all
// name: FUN_00401d29
// address: 0x401d29
// body: available
// range: 0x401d29-0x401dde inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


undefined4 __cdecl FUN_00401d29(char *param_1)

{
  undefined4 uVar1;
  DWORD DVar2;
  int local_14;
  DWORD local_10;
  uint local_c;
  ushort *local_8;
  
  local_8 = (ushort *)0x0;
  local_14 = DAT_0046487c;
  local_c = DAT_004654d8;
  if (DAT_00464880 == 0) {
    uVar1 = 1;
  }
  else {
    local_10 = timeGetTime();
    local_8 = FUN_0040f135(param_1,&local_14,&local_c,DAT_0045c040,DAT_00468ca0);
    DVar2 = timeGetTime();
    local_10 = DVar2 - local_10;
    if (local_8 == (ushort *)0x0) {
      uVar1 = 1;
    }
    else {
      FUN_0042382b(DAT_00464880);
      uVar1 = 0;
    }
  }
  return uVar1;
}

