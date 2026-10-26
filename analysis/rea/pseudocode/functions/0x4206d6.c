// rea decompile_all
// name: FUN_004206d6
// address: 0x4206d6
// body: available
// range: 0x4206d6-0x4207a5 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void FUN_004206d6(void)

{
  int iVar1;
  
  iVar1 = DAT_0046c884;
  FUN_0041f065(0);
  if ((iVar1 < DAT_0046c884) && (DAT_0046c5eb == '\x01')) {
    for (; (DAT_0045f7cc * 5 + 5) * 2 < DAT_0046c884; DAT_0045f7cc = DAT_0045f7cc + 1) {
    }
  }
  else if ((DAT_0046c884 < iVar1) && ((DAT_0046c884 < DAT_0045f7cc * 10 + 1 && (0 < DAT_0045f7cc))))
  {
    DAT_0045f7cc = DAT_0045f7cc + -1;
  }
  FUN_0041f41c();
  DAT_0045f7d0 = 0xffffffff;
  FUN_0041f279(-1);
  FUN_0042044d(1);
  FUN_004202ff(0x29,1);
  return;
}

