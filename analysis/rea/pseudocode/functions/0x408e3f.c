// rea decompile_all
// name: FUN_00408e3f
// address: 0x408e3f
// body: available
// range: 0x408e3f-0x408ee1 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


undefined4 FUN_00408e3f(void)

{
  undefined4 uVar1;
  int iVar2;
  
  if (DAT_0045d1b8 == 0) {
    iVar2 = FUN_00416710(s_setupart_top_bar_tga_0045d2e8,&DAT_0045d1b8,0,(uint *)0x0,(uint *)0x0);
    if (iVar2 == 0) {
      iVar2 = FUN_004200bb(0x468ce8,s_setupart_hotbar_rle_0045d300);
      if (iVar2 == 5) {
        iVar2 = FUN_004200bb(0x468d60,s_setupart_dwnbar_rle_0045d314);
        if (iVar2 == 5) {
          uVar1 = 0;
        }
        else {
          uVar1 = 1;
        }
      }
      else {
        uVar1 = 1;
      }
    }
    else {
      uVar1 = 1;
    }
  }
  else {
    uVar1 = 0;
  }
  return uVar1;
}

