// rea decompile_all
// name: FUN_004207ef
// address: 0x4207ef
// body: available
// range: 0x4207ef-0x420887 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_004207ef(int param_1,FILE *param_2)

{
  undefined4 uVar1;
  int *piVar2;
  undefined4 *puVar3;
  undefined4 local_1c;
  undefined4 local_18;
  undefined4 local_14;
  undefined4 local_10;
  int local_c;
  char *local_8;
  
  local_1c = 0;
  local_18 = 0;
  local_14 = 0xa8;
  local_10 = 0xa8;
  puVar3 = &local_1c;
  uVar1 = FUN_00418040(param_1);
  piVar2 = (int *)FUN_00418040((int)DAT_00477344);
  FUN_004207a6(piVar2,uVar1,puVar3);
  local_8 = DAT_00477344;
  for (local_c = 0; local_c < 0xa8; local_c = local_c + 1) {
    FUN_0044a460(local_8,2,0xa8,param_2);
    local_8 = local_8 + 0x500;
  }
  return;
}

