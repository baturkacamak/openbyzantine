// rea decompile_all
// name: FUN_004163f3
// address: 0x4163f3
// body: available
// range: 0x4163f3-0x41649d inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void FUN_004163f3(void)

{
  int unaff_EDI;
  uint local_108 [65];
  
  DAT_0045f050 = 0;
  FUN_0042b280(DAT_0046e82c,0x4b000);
  FUN_0041842b((int)DAT_0046e82c,0x4b000);
  FUN_0040c624(local_108,(uint *)s_video_0045f1c8,(uint *)&DAT_0045f1c4,(uint *)&DAT_0045f1bc);
  FUN_0040d3af(local_108,6,0,0x14,0,unaff_EDI);
  FUN_0040c624(local_108,(uint *)s_video_0045f1dc,(uint *)&DAT_0045f1d8,(uint *)&DAT_0045f1d0);
  FUN_0040d3af(local_108,6,0,0,0,unaff_EDI);
  return;
}

