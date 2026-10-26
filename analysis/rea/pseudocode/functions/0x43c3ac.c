// rea decompile_all
// name: FUN_0043c3ac
// address: 0x43c3ac
// body: available
// range: 0x43c3ac-0x43c465 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __cdecl FUN_0043c3ac(ushort *param_1)

{
  FUN_0043c2ef(*param_1);
  FUN_0043c1a8((short *)param_1);
  if ((DAT_0046edf4 == '\x01') && (DAT_0045dad8 = 1, DAT_0045dad4 != 0)) {
    DAT_0045dad4 = 0;
    FUN_0041842b(DAT_0046e82c,0x3e800);
  }
  else if (DAT_0046caf8 != 0xffff) {
    if (DAT_0046cb00 < 0x280) {
      DAT_0046cb00 = DAT_0046cb00 + 1;
    }
    _DAT_0046cb04 = _DAT_0046cb04 + 1;
    FUN_004181c5(DAT_00486888,&DAT_0046caf8,DAT_0046caf8,DAT_0046cafc);
  }
  return;
}

