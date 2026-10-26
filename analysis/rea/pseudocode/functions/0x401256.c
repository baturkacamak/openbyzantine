// rea decompile_all
// name: FUN_00401256
// address: 0x401256
// body: available
// range: 0x401256-0x401387 inclusive
// range: 0x40139f-0x4013ad inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


undefined4 FUN_00401256(void)

{
  void *this;
  undefined4 uVar1;
  int iVar2;
  undefined4 *unaff_FS_OFFSET;
  undefined4 *local_28;
  undefined4 local_18;
  ushort *local_14;
  undefined4 local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_00401395;
  local_10 = *unaff_FS_OFFSET;
  *unaff_FS_OFFSET = &local_10;
  FUN_004011de();
  DAT_00468b98 = 0;
  this = (void *)FUN_0044ac60(0x70);
  local_8 = 0;
  if (this == (void *)0x0) {
    local_28 = (undefined4 *)0x0;
  }
  else {
    local_28 = FUN_00423310(this,DAT_00486888,&DAT_0045c048);
  }
  local_8 = 0xffffffff;
  DAT_00464880 = local_28;
  if (local_28 == (undefined4 *)0x0) {
    uVar1 = 1;
  }
  else {
    local_14 = FUN_0040f135(s_nodes_sm_sm00_jpg_0045c0a4,&DAT_0046487c,(uint *)&DAT_004654d8,
                            (ushort *)0x0,0);
    if (local_14 == (ushort *)0x0) {
      uVar1 = 1;
    }
    else {
      iVar2 = FUN_004236b5(DAT_00464880,(int)local_14,DAT_0046487c,DAT_004654d8);
      FUN_0044ac40(local_14);
      if (iVar2 == 0) {
        FUN_00423d9e(DAT_00464880,&DAT_0045c040,&local_18,&local_18,&DAT_00468ca0);
        uVar1 = 0;
      }
      else {
        uVar1 = 1;
      }
    }
  }
  *unaff_FS_OFFSET = local_10;
  return uVar1;
}

