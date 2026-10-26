// rea decompile_all
// name: FUN_0042d915
// address: 0x42d915
// body: available
// range: 0x42d915-0x42da3c inclusive
// range: 0x42da42-0x42da61 inclusive
// range: 0x42dab5-0x42dad8 inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


ushort FUN_0042d915(void)

{
  ushort uVar1;
  ushort local_8;
  
  switch(*(undefined1 *)(DAT_0047a230 + 1)) {
  case 2:
    uVar1 = FUN_00436652(DAT_0047a230,0);
    local_8 = *(ushort *)
               (&DAT_0047e2f0 +
               (uint)*(ushort *)(&DAT_0046ee00 + (uint)uVar1 * 2 + (uint)DAT_00482428 * 600) * 2);
    break;
  case 8:
    local_8 = *(short *)(DAT_0047a230 + 10) * 0x100 + *(short *)(DAT_0047a230 + 0xc) + 0x80;
    break;
  case 0x10:
    local_8 = *(short *)(DAT_0047a230 + 10) * 0x100 + *(short *)(DAT_0047a230 + 0xc) + 0x40;
    break;
  case 0x40:
    local_8 = FUN_00436652(DAT_0047a230,0);
    if (2 < local_8) {
      FUN_0040ac43();
      FUN_0040ac38();
      local_8 = 0x8001;
    }
  }
  local_8 = local_8 | 0x8000;
  FUN_00437c72();
  return local_8;
}

