// rea decompile_all
// name: FUN_0041917b
// address: 0x41917b
// body: available
// range: 0x41917b-0x41920e inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void FUN_0041917b(void)

{
  int *piVar1;
  undefined4 uVar2;
  undefined4 *puVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  undefined4 local_24;
  undefined4 local_20;
  undefined4 local_1c;
  undefined4 local_18;
  undefined4 local_14;
  undefined4 local_10;
  undefined4 local_c;
  undefined4 local_8;
  
  piVar1 = (int *)FUN_00418040((int)DAT_00477344);
  local_24 = 0;
  local_20 = 0x14;
  local_1c = 0x280;
  local_18 = 0x1a4;
  local_14 = 0x80;
  local_10 = 0x94;
  local_c = 0x200;
  local_8 = 0x124;
  FUN_0042b280(DAT_00477344,0x4b000);
  uVar5 = 0;
  uVar4 = 0x1000000;
  puVar3 = &local_24;
  uVar2 = FUN_00418040(DAT_0046e82c);
  (**(code **)(*piVar1 + 0x14))(piVar1,&local_14,uVar2,puVar3,uVar4,uVar5);
  return;
}

