// rea decompile_all
// name: FUN_00409230
// address: 0x409230
// body: available
// range: 0x409230-0x4092af inclusive
// range: 0x4092b5-0x4092cc inclusive
// range: 0x4092dd-0x409340 inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void FUN_00409230(void)

{
  int local_8;
  
  for (local_8 = DAT_0047a89c; local_8 != 0; local_8 = *(int *)(local_8 + 8)) {
    FUN_004599c0((byte *)s_____>_UNDOING_0045d328);
    switch(*(undefined1 *)(local_8 + 4)) {
    case 0x4a:
      FUN_004599c0(&DAT_0045d338);
      break;
    case 0x4b:
      FUN_004599c0(&DAT_0045d340);
      break;
    case 0x4c:
      FUN_004599c0((byte *)s_IFTOFF_0045d344);
      break;
    case 0x4d:
      FUN_004599c0((byte *)s_IFTON_0045d34c);
    }
    FUN_004599c0(&DAT_0045d354);
    FUN_0040a470(*(byte *)(local_8 + 5),'\0');
    FUN_004599c0(&DAT_0045d358);
    FUN_004599c0(&DAT_0045d35c);
    FUN_004599c0(&DAT_0045d360);
  }
  return;
}

