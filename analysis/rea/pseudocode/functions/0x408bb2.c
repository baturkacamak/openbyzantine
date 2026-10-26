// rea decompile_all
// name: FUN_00408bb2
// address: 0x408bb2
// body: available
// range: 0x408bb2-0x408c80 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


ushort __cdecl FUN_00408bb2(ushort param_1)

{
  ushort uVar1;
  char *_Str2;
  int iVar2;
  
  if (DAT_00482540 != 0) {
    _Str2 = (char *)**(undefined4 **)(&DAT_00485400 + (uint)param_1 * 4);
    for (uVar1 = 0; (uVar1 < 0x15e && ((&DAT_00482540)[uVar1] != 0)); uVar1 = uVar1 + 1) {
      iVar2 = __strcmpi((char *)(&DAT_00482540)[uVar1],_Str2);
      if (iVar2 == 0) {
        return uVar1;
      }
    }
    FUN_0040ac43();
    param_1 = 0;
  }
  return param_1;
}

