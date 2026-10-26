// rea decompile_all
// name: FUN_0041893a
// address: 0x41893a
// body: available
// range: 0x41893a-0x418a1d inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_0041893a(int param_1,int param_2)

{
  int iVar1;
  tagRECT local_34;
  ushort *local_24;
  ushort *local_20;
  undefined4 local_1c;
  int local_18;
  LONG local_14;
  LONG local_10;
  LONG local_c;
  LONG local_8;
  
  local_14 = 0;
  local_10 = 0x14;
  local_c = 0x280;
  local_8 = 0x1a4;
  local_1c = FUN_00418040(param_2);
  local_24 = (ushort *)(param_2 + 0x6400);
  local_20 = (ushort *)(param_1 + 0x6400);
  iVar1 = FUN_00416220();
  if (iVar1 == 0) {
    FUN_0041809a(&local_34);
  }
  else {
    local_34.left = local_14;
    local_34.top = local_10;
    local_34.right = local_c;
    local_34.bottom = local_8;
  }
  for (local_18 = 2; local_18 < 9; local_18 = local_18 + 2) {
    FUN_0042b4be(local_20,local_24,local_18,0x3e800);
    (**(code **)(*DAT_004869d4 + 0x14))(DAT_004869d4,&local_34,local_1c,&local_14,0x1000000,0);
  }
  return;
}

