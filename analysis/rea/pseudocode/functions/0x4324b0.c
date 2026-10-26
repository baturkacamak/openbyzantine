// rea decompile_all
// name: FUN_004324b0
// address: 0x4324b0
// body: available
// range: 0x4324b0-0x4325d2 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_004324b0(uint param_1,int param_2)

{
  short *psVar1;
  short sVar2;
  
  psVar1 = *(short **)(&DAT_004649e8 + (param_1 & 0xffff) * 4);
  if (((&DAT_00460428)[*(ushort *)(param_2 + 0xe)] != ' ') ||
     ((*(byte *)((int)psVar1 + 9) & 0x20) == 0)) {
    if (*(char *)(param_2 + 2) != '\0') {
      *(undefined *)((int)psVar1 + 9) = (&DAT_00460428)[*(ushort *)(param_2 + 0xe)];
    }
    if (*(char *)(param_2 + 3) != '\0') {
      sVar2 = FUN_00436652(param_2,2);
      psVar1[2] = sVar2;
    }
    if (*(char *)(param_2 + 4) != '\0') {
      sVar2 = FUN_00436652(param_2,3);
      *psVar1 = sVar2;
    }
    if (*(char *)(param_2 + 5) != '\0') {
      sVar2 = FUN_00436652(param_2,4);
      psVar1[1] = sVar2 + 0x14;
    }
    *(undefined4 *)(psVar1 + 0xe) = *(undefined4 *)(psVar1 + 0xc);
    psVar1[0x10] = 0;
    psVar1[0x11] = 0;
    psVar1[0x12] = *psVar1;
    psVar1[0x13] = psVar1[1];
    psVar1[3] = 0;
  }
  return;
}

