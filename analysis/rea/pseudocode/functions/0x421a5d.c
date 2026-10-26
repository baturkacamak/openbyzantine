// rea decompile_all
// name: FUN_00421a5d
// address: 0x421a5d
// body: available
// range: 0x421a5d-0x421b8b inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


int __cdecl FUN_00421a5d(int param_1,byte param_2)

{
  uint *puVar1;
  int iVar2;
  int local_20;
  int local_1c;
  int local_14;
  uint *local_10;
  int local_8;
  
  if ((param_2 & 1) == 0) {
    local_14 = 0x61;
    local_8 = 0x165;
  }
  else {
    local_14 = 0x69;
    local_8 = 0x1b3;
  }
  local_1c = 0;
  while( true ) {
    if (*(int *)(param_1 + 4) <= local_1c) {
      return local_1c;
    }
    puVar1 = *(uint **)(*(int *)(param_1 + 0x10) + local_1c * 4);
    iVar2 = FUN_0040701f(puVar1,0xb9,2);
    local_10 = puVar1;
    for (local_20 = 0; (local_20 < iVar2 && (local_20 * 0x14 + local_14 + 0x14 <= local_8));
        local_20 = local_20 + 1) {
      local_10 = (uint *)FUN_0044c520((char *)((int)local_10 + 1),'\n');
    }
    FUN_0040711b((char *)puVar1);
    if (local_20 < iVar2) break;
    local_14 = local_14 + 0x14 + iVar2 * 0x14;
    local_1c = local_1c + 1;
  }
  if (local_20 < 1) {
    return local_1c;
  }
  if (local_10 == puVar1) {
    return local_1c;
  }
  *(undefined1 *)local_10 = 10;
  return local_1c + 1;
}

