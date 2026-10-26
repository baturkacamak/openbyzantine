// rea decompile_all
// name: FUN_00413ac0
// address: 0x413ac0
// body: available
// range: 0x413ac0-0x413b5a inclusive
// range: 0x413b72-0x413b80 inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


bool __cdecl
FUN_00413ac0(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
            int *param_5,undefined4 param_6)

{
  void *this;
  undefined4 *unaff_FS_OFFSET;
  undefined4 *local_18;
  undefined4 local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_00413b68;
  local_10 = *unaff_FS_OFFSET;
  *unaff_FS_OFFSET = &local_10;
  this = (void *)FUN_0044ac60(0x98);
  local_8 = 0;
  if (this == (void *)0x0) {
    local_18 = (undefined4 *)0x0;
  }
  else {
    local_18 = FUN_004397c0(this,param_1,param_2,param_3,param_4,param_5,param_6);
  }
  DAT_0046c4dc = local_18;
  *unaff_FS_OFFSET = local_10;
  return local_18 == (undefined4 *)0x0;
}

