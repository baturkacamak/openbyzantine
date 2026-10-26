// rea decompile_all
// name: FUN_00412d35
// address: 0x412d35
// body: available
// range: 0x412d35-0x412f76 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_00412d35(uint *param_1)

{
  undefined4 local_18;
  uint local_14 [4];
  
  FUN_0044ba00(local_14,param_1);
  local_18 = FUN_0044c580(local_14);
  local_18[(int)&local_18 + 2] = '\0';
  FUN_0044ba00((uint *)&DAT_004859a0,local_14);
  FUN_0044ba08((uint *)&DAT_004859a0,(uint *)&DAT_0045eaf0);
  FUN_0044ba00((uint *)&DAT_0046e800,local_14);
  FUN_0044ba08((uint *)&DAT_0046e800,(uint *)&DAT_0045eaf4);
  FUN_0044ba00((uint *)&DAT_004859e0,local_14);
  FUN_0044ba08((uint *)&DAT_004859e0,(uint *)&DAT_0045eaf8);
  FUN_0044ba00((uint *)&DAT_00484310,local_14);
  FUN_0044ba08((uint *)&DAT_00484310,(uint *)&DAT_0045eafc);
  FUN_0044ba00((uint *)&DAT_0047a250,local_14);
  FUN_0044ba08((uint *)&DAT_0047a250,(uint *)&DAT_0045eb00);
  FUN_0044ba00((uint *)&DAT_00483630,local_14);
  FUN_0044ba08((uint *)&DAT_00483630,(uint *)&DAT_0045eb04);
  FUN_0044ba00((uint *)&DAT_0047a2a0,local_14);
  FUN_0044ba08((uint *)&DAT_0047a2a0,(uint *)&DAT_0045eb08);
  FUN_0044ba00((uint *)&DAT_0047fa60,local_14);
  FUN_0044ba08((uint *)&DAT_0047fa60,(uint *)&DAT_0045eb0c);
  FUN_0044ba00((uint *)&DAT_0046e240,local_14);
  FUN_0044ba08((uint *)&DAT_0046e240,(uint *)&DAT_0045eb10);
  FUN_0044ba00((uint *)&DAT_0047a270,local_14);
  FUN_0044ba08((uint *)&DAT_0047a270,(uint *)&DAT_0045eb14);
  FUN_0044ba00((uint *)&DAT_0047ae60,local_14);
  FUN_0044ba08((uint *)&DAT_0047ae60,(uint *)&DAT_0045eb18);
  FUN_0044ba00((uint *)&DAT_00482440,local_14);
  FUN_0044ba08((uint *)&DAT_00482440,(uint *)&DAT_0045eb1c);
  FUN_0044ba00((uint *)&DAT_00480320,local_14);
  FUN_0044ba08((uint *)&DAT_00480320,(uint *)&DAT_0045eb20);
  FUN_0044ba00((uint *)&DAT_00483610,local_14);
  FUN_0044ba08((uint *)&DAT_00483610,(uint *)&DAT_0045eb24);
  FUN_0044ba00((uint *)&DAT_00483060,local_14);
  FUN_0044ba08((uint *)&DAT_00483060,(uint *)&DAT_0045eb28);
  return;
}

