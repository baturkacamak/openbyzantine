// rea decompile_all
// name: FUN_0040f524
// address: 0x40f524
// body: available
// range: 0x40f524-0x40f6a4 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0040f524(void)

{
  ushort uVar1;
  ushort uVar2;
  
  if (DAT_00485c6c != '\0') {
    _DAT_0047dd60 = 0;
    for (uVar1 = 0; uVar1 < 6; uVar1 = uVar1 + 1) {
      for (uVar2 = 0; uVar2 < 0x15f; uVar2 = uVar2 + 1) {
        *(undefined4 *)(&DAT_00480340 + (uint)uVar2 * 4 + (uint)uVar1 * 0x57c) = 0;
      }
    }
  }
  for (uVar1 = 0; uVar1 < 0x15e; uVar1 = uVar1 + 1) {
    *(undefined4 *)(&DAT_00464300 + (uint)uVar1 * 4) = 0;
    *(undefined4 *)(&DAT_00464f60 + (uint)uVar1 * 4) = 0;
    *(undefined4 *)(&DAT_004649e8 + (uint)uVar1 * 4) = 0;
    *(undefined4 *)(&DAT_00482ad0 + (uint)uVar1 * 4) = 0;
    *(undefined4 *)(&DAT_00486220 + (uint)uVar1 * 4) = 0;
    *(undefined4 *)(&DAT_00486a10 + (uint)uVar1 * 4) = 0;
    (&DAT_00464888)[uVar1] = 0;
  }
  for (uVar1 = 0; uVar1 < 0xdac; uVar1 = uVar1 + 1) {
    *(undefined4 *)(&DAT_004654e0 + (uint)uVar1 * 4) = 0;
  }
  return;
}

