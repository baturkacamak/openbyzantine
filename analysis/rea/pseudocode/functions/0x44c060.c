// rea decompile_all
// name: FUN_0044c060
// address: 0x44c060
// body: available
// range: 0x44c060-0x44c0df inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; Ghidra Listing reports a terminal call at 0x44c0d7 to EXTERNAL:0x2d KERNEL32.DLL::ExitProcess (external=true, thunk=false, hasNoReturn=true); fallthrough is excluded by Ghidra's flow model. This records Ghidra's FunctionManager flag, not an independent verification that the callee cannot return.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __cdecl FUN_0044c060(UINT param_1,int param_2,int param_3)

{
  undefined4 *puVar1;
  
  _DAT_00462cb8 = 1;
  DAT_00462cb4 = (undefined1)param_3;
  if (param_2 == 0) {
    if ((DAT_004a8834 != (undefined4 *)0x0) &&
       (puVar1 = (undefined4 *)(DAT_004a8830 + -4), DAT_004a8834 <= puVar1)) {
      do {
        if ((code *)*puVar1 != (code *)0x0) {
          (*(code *)*puVar1)();
        }
        puVar1 = puVar1 + -1;
      } while (DAT_004a8834 <= puVar1);
    }
    FUN_0044c0e0((undefined4 *)&DAT_0045c020,(undefined4 *)&DAT_0045c02c);
  }
  FUN_0044c0e0((undefined4 *)&DAT_0045c030,(undefined4 *)&DAT_0045c034);
  if (param_3 == 0) {
                    /* WARNING: Subroutine does not return */
    ExitProcess(param_1);
  }
  return;
}

