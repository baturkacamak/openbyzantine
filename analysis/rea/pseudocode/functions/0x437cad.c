// rea decompile_all
// name: FUN_00437cad
// address: 0x437cad
// body: available
// range: 0x437cad-0x437f75 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


int FUN_00437cad(void)

{
  uint uVar1;
  char *pcVar2;
  uint local_c;
  int local_8;
  
  local_8 = 0;
  for (local_c = 0; ((&DAT_0046e260)[local_c] != '\0' && ((&DAT_0046e260)[local_c] != '.'));
      local_c = local_c + 1) {
  }
  if ((&DAT_0046e260)[local_c] == '\0') {
    FUN_0044ba08((uint *)&DAT_0046e260,(uint *)&DAT_00461524);
    local_8 = FUN_00437f76(&local_c);
  }
  else {
    uVar1 = local_c + 1;
    if ((&DAT_0046e261)[local_c] == '\0') {
      local_c = uVar1;
      local_8 = FUN_00437f76(&local_c);
    }
    else if (((((&DAT_0046e261)[local_c] & 0xdf) == 0x4c) &&
             (((&DAT_0046e262)[local_c] & 0xdf) == 0x43)) &&
            (((&DAT_0046e263)[local_c] & 0xdf) == 0x43)) {
      local_c = uVar1;
      FUN_0044ba00((uint *)&DAT_0046e250,(uint *)&DAT_0046e260);
      FUN_0044ba00((uint *)&DAT_00485980,(uint *)&DAT_0046e260);
      FUN_0044ba00((uint *)&DAT_00484320,(uint *)&DAT_0046e260);
      FUN_0044ba00((uint *)&DAT_0047a290,(uint *)&DAT_0046e260);
      FUN_0044ba00((uint *)&DAT_00482430,(uint *)&DAT_0046e260);
      FUN_0044ba00((uint *)&DAT_00482ac0,(uint *)&DAT_0046e260);
      pcVar2 = FUN_0044c580((uint *)&DAT_00485980);
      pcVar2[0x48597f] = 'v';
      pcVar2 = FUN_0044c580((uint *)&DAT_00484320);
      pcVar2[0x48431e] = 'v';
      pcVar2 = FUN_0044c580((uint *)&DAT_00484320);
      pcVar2[0x48431f] = 'm';
      pcVar2 = FUN_0044c580((uint *)&DAT_0047a290);
      pcVar2[0x47a28f] = 'x';
      pcVar2 = FUN_0044c580((uint *)&DAT_00482430);
      pcVar2[0x48242f] = 'i';
      pcVar2 = FUN_0044c580((uint *)&DAT_00482ac0);
      pcVar2[0x482abf] = 'y';
      local_8 = FUN_00438173();
      if (local_8 == 0xff) {
        FUN_0040ac43();
        FUN_0040ac38();
        local_8 = 1;
      }
    }
    else if (((((&DAT_0046e261)[local_c] & 0xdf) != 0x43) ||
             (((&DAT_0046e262)[local_c] & 0xdf) != 0x43)) ||
            (((&DAT_0046e263)[local_c] & 0xdf) != 0)) {
      local_c = uVar1;
      FUN_0040ac43();
      FUN_0040ac38();
      local_8 = 1;
    }
  }
  return local_8;
}

