// rea decompile_all
// name: FUN_00437f76
// address: 0x437f76
// body: available
// range: 0x437f76-0x4380fe inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


int __cdecl FUN_00437f76(uint *param_1)

{
  int local_8;
  
  *param_1 = *param_1 + 1;
  if (*param_1 < 10) {
    FUN_0044ba00((uint *)&DAT_0046e250,(uint *)&DAT_0046e260);
    FUN_0044ba00((uint *)&DAT_00485980,(uint *)&DAT_0046e260);
    FUN_0044ba00((uint *)&DAT_00484320,(uint *)&DAT_0046e260);
    FUN_0044ba00((uint *)&DAT_0047a290,(uint *)&DAT_0046e260);
    FUN_0044ba00((uint *)&DAT_00482530,(uint *)&DAT_0046e260);
    FUN_0044ba00((uint *)&DAT_00482430,(uint *)&DAT_0046e260);
    FUN_0044ba00((uint *)&DAT_00482ac0,(uint *)&DAT_0046e260);
    FUN_0044ba08((uint *)&DAT_0046e250,(uint *)&DAT_004615ac);
    FUN_0044ba08((uint *)&DAT_00485980,(uint *)&DAT_004615b0);
    FUN_0044ba08((uint *)&DAT_00484320,(uint *)&DAT_004615b4);
    FUN_0044ba08((uint *)&DAT_0047a290,(uint *)&DAT_004615b8);
    FUN_0044ba08((uint *)&DAT_00482530,(uint *)&DAT_004615bc);
    FUN_0044ba08((uint *)&DAT_00482430,(uint *)&DAT_004615c0);
    FUN_0044ba08((uint *)&DAT_00482ac0,(uint *)&DAT_004615c4);
    local_8 = FUN_00438173();
    if (local_8 == 0xff) {
      FUN_0044ba08((uint *)&DAT_0046e260,(uint *)&DAT_004615c8);
      local_8 = 0;
    }
  }
  else {
    FUN_0040ac43();
    FUN_0040ac38();
    local_8 = 1;
  }
  return local_8;
}

