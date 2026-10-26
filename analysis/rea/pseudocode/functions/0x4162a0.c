// rea decompile_all
// name: FUN_004162a0
// address: 0x4162a0
// body: available
// range: 0x4162a0-0x416374 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_004162a0(void)

{
  int iVar1;
  undefined4 uVar2;
  undefined4 local_24 [4];
  ushort local_14;
  ushort local_10;
  ushort local_c;
  
  local_24[0] = 0x20;
  iVar1 = (**(code **)(*DAT_004869d4 + 0x54))(DAT_004869d4,local_24);
  if (iVar1 == 0) {
    DAT_00485c7c = local_14;
    DAT_00485c86 = local_10;
    DAT_00485c84 = local_c;
    _DAT_00485c78 = FUN_00416375(local_14);
    _DAT_00485c80 = FUN_00416375(DAT_00485c86);
    DAT_00485c8c = FUN_00416375(DAT_00485c84);
    if (DAT_00485c86 == 0x7e0) {
      DAT_00485c86 = 0x7c0;
      _DAT_00485c80 = _DAT_00485c80 + 1;
    }
    DAT_00485c88 = (ushort)(0x1f << (DAT_00485c78 & 0x1f)) | 0x1f;
    uVar2 = 0;
  }
  else {
    uVar2 = 1;
  }
  return uVar2;
}

