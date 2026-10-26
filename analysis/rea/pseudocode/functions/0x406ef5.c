// rea decompile_all
// name: FUN_00406ef5
// address: 0x406ef5
// body: available
// range: 0x406ef5-0x406f98 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void FUN_00406ef5(void)

{
  byte bVar1;
  byte bVar2;
  byte bVar3;
  byte bVar4;
  
  bVar1 = DAT_00485c80 & 0x1f;
  bVar2 = DAT_00485c78 & 0x1f;
  bVar3 = DAT_00485c80 & 0x1f;
  bVar4 = DAT_00485c78 & 0x1f;
  DAT_00468cb8 = FUN_00405db2(s_fonts_white_sfn_0045d188);
  FUN_00405e7e((int)DAT_00468cb8,5,(ushort)(0xe << bVar1) | (ushort)(0xf << bVar2) | 0xe,0);
  DAT_00468cbc = FUN_00405db2(s_fonts_yellow_sfn_0045d198);
  FUN_00405e7e((int)DAT_00468cbc,6,(ushort)(0xe << bVar3) | (ushort)(0xe << bVar4) | 0xe,0);
  return;
}

