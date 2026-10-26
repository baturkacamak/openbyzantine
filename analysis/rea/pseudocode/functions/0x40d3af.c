// rea decompile_all
// name: FUN_0040d3af
// address: 0x40d3af
// body: available
// range: 0x40d3af-0x40d47d inclusive
// range: 0x40d483-0x40d496 inclusive
// range: 0x40d4b3-0x40d54c inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


undefined4 __cdecl
FUN_0040d3af(undefined4 param_1,int param_2,int param_3,int param_4,int param_5,int param_6)

{
  int iVar1;
  bool bVar2;
  int local_8;
  
  FUN_0040d9f7();
  switch(param_2) {
  case 0:
  case 3:
    param_4 = param_4 + 0x14;
    local_8 = 1;
    break;
  case 1:
    FUN_00419d28(DAT_0047a8a4);
    FUN_0042b280(DAT_0046e82c,0x4b000);
    param_4 = param_4 + 0x14;
  case 6:
    local_8 = 2;
    break;
  case 2:
  case 4:
    iVar1 = FUN_0041d440(1);
    if ((iVar1 == 0) && (param_6 != 0)) {
      FUN_004196a4(0,DAT_00477344,0);
      iVar1 = FUN_00416220();
      if (iVar1 == 0) {
        FUN_0042b280(DAT_0046e82c,0x41a00);
      }
    }
    param_4 = param_4 + 0x14;
    local_8 = 2;
    break;
  case 5:
    param_4 = param_4 + 0x14;
  default:
    local_8 = 0;
  }
  if ((local_8 == 2) && ((iVar1 = FUN_0041d440(1), iVar1 == 0 || (param_2 == 6)))) {
    local_8 = 0;
    param_3 = param_3 + 0xa0;
    param_4 = param_4 + 100;
  }
  FUN_0041624a();
  bVar2 = DAT_0045d1d0 != 0;
  if (bVar2) {
    DAT_0045d1d0 = 0;
  }
  FUN_00413bdd(param_1,param_3,param_4,local_8,param_5);
  if (bVar2) {
    DAT_0045d1d0 = 1;
  }
  return 1;
}

