// rea decompile_all
// name: FUN_0040d54d
// address: 0x40d54d
// body: available
// range: 0x40d54d-0x40d5fe inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl
FUN_0040d54d(ushort *param_1,ushort *param_2,int param_3,int param_4,int param_5,int param_6)

{
  ushort uVar1;
  int local_18;
  int local_10;
  ushort *local_c;
  ushort *local_8;
  
  local_8 = param_2;
  if (param_6 == 0) {
    _memset(param_1,0,0x7d000);
  }
  else {
    local_c = param_1;
  }
  for (local_10 = 0; local_10 < param_4; local_10 = local_10 + 1) {
    for (local_18 = 0; local_18 < param_3; local_18 = local_18 + 1) {
      uVar1 = FUN_004164a0(*local_8);
      *local_c = uVar1;
      local_c = local_c + 1;
      local_8 = local_8 + 1;
    }
    local_c = local_c + (param_5 - param_3);
  }
  return;
}

