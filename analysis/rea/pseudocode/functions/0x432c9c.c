// rea decompile_all
// name: FUN_00432c9c
// address: 0x432c9c
// body: available
// range: 0x432c9c-0x432d13 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_00432c9c(uint param_1)

{
  byte bVar1;
  int iVar2;
  
  iVar2 = *(int *)(&DAT_004649e8 + (param_1 & 0xffff) * 4);
  bVar1 = *(byte *)(iVar2 + 9);
  if ((((bVar1 & 0x40) != 0) || (((bVar1 & 0x20) != 0 && (*(int *)(iVar2 + 0x20) != 0)))) ||
     (((bVar1 & 4) == 0 && ((bVar1 & 8) != 0)))) {
    *(undefined1 *)(iVar2 + 0x29) = 1;
    DAT_00482acd = 1;
  }
  return;
}

