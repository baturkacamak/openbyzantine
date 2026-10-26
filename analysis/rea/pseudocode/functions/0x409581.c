// rea decompile_all
// name: FUN_00409581
// address: 0x409581
// body: available
// range: 0x409581-0x40963b inclusive
// range: 0x409641-0x409660 inclusive
// range: 0x4096b4-0x4096c5 inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_00409581(int param_1)

{
  FUN_00409457(s_RETURN_0045d3c4,'\x01');
  switch(*(undefined1 *)(param_1 + 1)) {
  case 2:
    FUN_0040a4bc(param_1,0,'\0','\x01');
    break;
  case 8:
    FUN_0040a470((byte)*(undefined2 *)(param_1 + 10),'\0');
    FUN_004599c0(&DAT_0045d3d0);
    break;
  case 0x10:
    FUN_0040a470((byte)*(undefined2 *)(param_1 + 10),'\0');
    FUN_004599c0(&DAT_0045d3cc);
    FUN_0040a470((byte)*(undefined2 *)(param_1 + 0xc),'\0');
    break;
  case 0x40:
    FUN_0040a40d(param_1,0,'\0');
  }
  FUN_004094f2();
  return;
}

