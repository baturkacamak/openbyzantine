// rea decompile_all
// name: FUN_00431a69
// address: 0x431a69
// body: available
// range: 0x431a69-0x431b0e inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


undefined4 __cdecl FUN_00431a69(int param_1)

{
  bool bVar1;
  undefined3 extraout_var;
  int iVar2;
  undefined4 uVar3;
  
  bVar1 = false;
  if (param_1 == 7) {
    bVar1 = FUN_0040c6d6(1);
    if (CONCAT31(extraout_var,bVar1) != 0) {
      DAT_0045da78 = 1;
      return 0xffff;
    }
    param_1 = 2;
    bVar1 = true;
  }
  iVar2 = FUN_0040c7b4(param_1);
  if (iVar2 == 0) {
    uVar3 = 0xffff;
  }
  else {
    if (bVar1) {
      DAT_0045f490 = 1;
    }
    else {
      DAT_0045da78 = param_1;
      DAT_0045f490 = 3;
    }
    uVar3 = 1;
  }
  return uVar3;
}

