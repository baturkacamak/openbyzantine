// rea decompile_all
// name: FUN_00407254
// address: 0x407254
// body: available
// range: 0x407254-0x4073c2 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


undefined4 __cdecl FUN_00407254(int param_1)

{
  undefined4 uVar1;
  int iVar2;
  uint local_124;
  undefined1 local_120 [8];
  undefined1 local_118;
  uint local_114;
  char *local_110;
  int local_10c;
  uint local_108 [65];
  
  if (param_1 == 0) {
    uVar1 = 1;
  }
  else if (*(int *)(param_1 + 0xc) == 0) {
    local_10c = *(int *)(param_1 + 8);
    if ((&DAT_00482540)[local_10c] == 0) {
      local_110 = (char *)**(undefined4 **)(&DAT_00485400 + local_10c * 4);
    }
    else {
      local_110 = (char *)(&DAT_00482540)[local_10c];
    }
    FUN_0044c630(local_120,local_110,8);
    local_118 = 0;
    FUN_0040c624(local_108,(uint *)s_inventor_0045d200,(uint *)local_120,(uint *)&DAT_0045d1f8);
    iVar2 = FUN_00416710((char *)local_108,(undefined4 *)(param_1 + 0xc),1,&local_124,&local_114);
    if (iVar2 == 0) {
      local_120[0] = 'h';
      FUN_0040c624(local_108,(uint *)s_inventor_hotitems_0045d214,(uint *)local_120,
                   (uint *)&DAT_0045d20c);
      iVar2 = FUN_00416710((char *)local_108,(undefined4 *)(param_1 + 0x14),1,&local_124,&local_114)
      ;
      if (iVar2 == 0) {
        *(undefined4 *)(param_1 + 0x10) = *(undefined4 *)(param_1 + 0xc);
        uVar1 = 0;
      }
      else {
        uVar1 = 1;
      }
    }
    else {
      uVar1 = 1;
    }
  }
  else {
    uVar1 = 0;
  }
  return uVar1;
}

