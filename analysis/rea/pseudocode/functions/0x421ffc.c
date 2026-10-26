// rea decompile_all
// name: FUN_00421ffc
// address: 0x421ffc
// body: available
// range: 0x421ffc-0x422100 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_00421ffc(int param_1,int param_2)

{
  int local_78;
  int local_74;
  uint local_6c [20];
  int *local_1c;
  int local_18;
  int local_14;
  undefined4 local_10;
  undefined4 local_c;
  uint local_8;
  
  local_74 = 0x55;
  local_18 = 0x172;
  local_14 = 0x55;
  local_10 = 0x22e;
  local_c = 0x16d;
  local_1c = (int *)FUN_00418040(DAT_00477344);
  if (param_2 != 0) {
    FUN_00420683(&local_18);
  }
  for (local_78 = 0x34; (local_78 < 0x3b && (local_78 <= DAT_0045f7dc)); local_78 = local_78 + 1) {
    local_8 = (uint)(local_78 == param_1);
    FUN_0041d45a(local_6c,local_78);
    FUN_004063e0((char *)local_6c,0x1d0,local_74,2,local_1c,local_8,1,1);
    local_74 = local_74 + 0x28;
  }
  if (param_2 != 0) {
    FUN_004181c5(local_1c,&local_18,local_18,local_14);
  }
  return;
}

