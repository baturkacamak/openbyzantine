// rea decompile_all
// name: FUN_0040c911
// address: 0x40c911
// body: available
// range: 0x40c911-0x40c9cd inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


int FUN_0040c911(void)

{
  DWORD DVar1;
  UINT UVar2;
  int local_20;
  int local_14;
  char local_10 [12];
  
  local_14 = 0;
  DAT_0045da7c = 0;
  DVar1 = GetLogicalDrives();
  local_20 = 0x44;
  do {
    if (0x5a < local_20) {
      return local_14;
    }
    local_10[0] = (char)local_20;
    if ((DVar1 & 1 << (local_10[0] + 0xbfU & 0x1f)) != 0) {
      local_10[1] = 0x3a;
      local_10[2] = 0x5c;
      local_10[3] = 0;
      UVar2 = GetDriveTypeA(local_10);
      if (UVar2 == 5) {
        DAT_0045da7c = DAT_0045da7c + 1;
        if (local_14 == 0) {
          local_14 = local_20;
        }
      }
      else if (local_14 != 0) {
        return local_14;
      }
    }
    local_20 = local_20 + 1;
  } while( true );
}

