// rea decompile_all
// name: FUN_0040dae5
// address: 0x40dae5
// body: available
// range: 0x40dae5-0x40dd7e inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_0040dae5(int param_1)

{
  undefined4 uVar1;
  int iVar2;
  uint local_134;
  int local_130;
  int local_12c;
  uint local_128;
  char local_124 [260];
  int local_20;
  char *local_1c;
  undefined4 local_18 [4];
  int local_8;
  
  local_20 = 0;
  for (local_12c = 0; local_12c < 4; local_12c = local_12c + 1) {
    uVar1 = FUN_0044c2f0(0x105);
    local_18[local_12c] = uVar1;
  }
  if ((DAT_00476338 < 0x20) && (1 < *(int *)(PTR_PTR_0045e534 + (uint)DAT_00476338 * 0x24 + 0x10)))
  {
    FUN_0040ed73();
  }
  FUN_0040da4c();
  if (param_1 == -1) {
    PTR_PTR_0045e534 = (undefined *)&PTR_s_default_0045dbe8;
    local_1c = s_cursors__s__tga_0045e6ec;
  }
  else {
    PTR_PTR_0045e534 = (undefined *)&PTR_s_default_0045e090;
    local_1c = s_cursors_klio__s__tga_0045e6fc;
  }
  for (local_12c = 0; local_12c < 0x20; local_12c = local_12c + 1) {
    if (*(int *)(PTR_PTR_0045e534 + local_12c * 0x24) != 0) {
      _sprintf(local_124,local_1c,*(undefined4 *)(PTR_PTR_0045e534 + local_12c * 0x24));
      local_8 = FUN_0040e89f(local_124,(int)local_18,4);
      if (local_8 == 0) {
        local_20 = 1;
      }
      for (local_130 = 0; local_130 < local_8; local_130 = local_130 + 1) {
        iVar2 = FUN_00416710((char *)local_18[local_130],
                             (undefined4 *)
                             (PTR_PTR_0045e534 + local_12c * 0x24 + local_130 * 4 + 0x14),0,
                             &local_134,&local_128);
        local_20 = local_20 + iVar2;
        *(undefined4 *)(PTR_PTR_0045e534 + local_12c * 0x24 + 0xc) = 0;
      }
      *(int *)(PTR_PTR_0045e534 + local_12c * 0x24 + 0x10) = local_8;
    }
  }
  DAT_0046c45c = 0x30;
  DAT_0046c458 = 0x30;
  iVar2 = (**(code **)(*DAT_0045dbd4 + 0x1c))
                    (DAT_0045dbd4,0,0,*(undefined4 *)(PTR_PTR_0045e534 + 0x14),&DAT_0046c450,0x10);
  local_20 = local_20 + iVar2;
  DAT_00476338 = 0;
  for (local_12c = 0; local_12c < 4; local_12c = local_12c + 1) {
    FUN_0044ac40((LPVOID)local_18[local_12c]);
  }
  return;
}

