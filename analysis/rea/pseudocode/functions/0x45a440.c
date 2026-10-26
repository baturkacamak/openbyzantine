// rea decompile_all
// name: FUN_0045a440
// address: 0x45a440
// body: available
// range: 0x45a440-0x45a481 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


uint __cdecl FUN_0045a440(byte param_1)

{
  BOOL BVar1;
  byte local_5;
  DWORD local_4;
  
  local_5 = param_1;
  if (DAT_004642e8 != (HANDLE)0xffffffff) {
    BVar1 = WriteConsoleA(DAT_004642e8,&local_5,1,&local_4,(LPVOID)0x0);
    if (BVar1 != 0) {
      return (uint)local_5;
    }
  }
  return 0xffffffff;
}

