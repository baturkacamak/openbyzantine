// rea decompile_all
// name: FUN_0040a615
// address: 0x40a615
// body: available
// range: 0x40a615-0x40a6a4 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_0040a615(undefined4 param_1,int param_2,char param_3,char param_4)

{
  if (DAT_00485c6c != '\0') {
    FUN_00409457(param_1,'\x01');
    FUN_0040a4bc(param_2,0,'\0','\x01');
    FUN_0040a40d(param_2,1,'\x01');
    if (param_4 == '\x01') {
      FUN_0040a470((byte)*(undefined2 *)(param_2 + 0x12),'\x01');
    }
    else {
      FUN_0040a574(param_2,2,'\x01',param_3);
    }
    FUN_004094f2();
  }
  return;
}

