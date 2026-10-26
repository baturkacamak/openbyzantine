// rea decompile_all
// name: FUN_00420b99
// address: 0x420b99
// body: available
// range: 0x420b99-0x420c50 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int FUN_00420b99(void)

{
  int iVar1;
  undefined4 local_7c;
  undefined4 local_78;
  undefined4 local_74;
  undefined4 local_70;
  undefined4 local_14;
  int local_10;
  int local_c;
  
  local_c = 0;
  _memset(&local_7c,0,0x6c);
  local_7c = 0x6c;
  local_78 = 7;
  local_14 = 0x840;
  local_70 = 0xa8;
  local_74 = 0xa8;
  local_10 = (**(code **)(*DAT_00486a04 + 0x18))(DAT_00486a04,&local_7c,&DAT_0045f7c8,0);
  if (local_10 == 0) {
    iVar1 = FUN_004201cc();
    local_c = local_c + iVar1;
    iVar1 = FUN_0041d7ff();
    local_c = local_c + iVar1;
    DAT_0045f7dc = FUN_004273f4();
    _DAT_0045f794 = DAT_0045f78c + (DAT_0045f7dc + -0x33) * 0x28;
    local_10 = local_c;
  }
  return local_10;
}

