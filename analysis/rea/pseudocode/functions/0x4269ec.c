// rea decompile_all
// name: FUN_004269ec
// address: 0x4269ec
// body: available
// range: 0x4269ec-0x426b9c inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


int __cdecl FUN_004269ec(FILE *param_1)

{
  char *pcVar1;
  undefined4 *puVar2;
  int iVar3;
  int local_18;
  int local_14;
  byte local_10;
  int local_c;
  
  local_14 = 0;
  local_18 = 0;
  while ((local_18 < 0x15e && (local_14 == 0))) {
    pcVar1 = *(char **)(&DAT_004649e8 + local_18 * 4);
    if (pcVar1 == (char *)0x0) {
      _fputc(0xff,param_1);
    }
    else {
      if ((*(char *)(*(int *)(&DAT_0047dd70 + local_18 * 4) + 8) == '\0') ||
         (*(char *)(*(int *)(&DAT_0047dd70 + local_18 * 4) + 8) == '\x01')) {
        _fputc(1,param_1);
      }
      else {
        _fputc(0,param_1);
      }
      FUN_0044a460(pcVar1,0x40,1,param_1);
      FUN_004259de((int)pcVar1,param_1);
      local_14 = FUN_004269a0(*(undefined4 *)(pcVar1 + 0x2c),param_1);
      iVar3 = FUN_004269a0(*(undefined4 *)(pcVar1 + 0x30),param_1);
      local_14 = local_14 + iVar3;
      if (*(int *)(pcVar1 + 0x34) == 0) {
        _fputc(0,param_1);
      }
      else {
        local_10 = 0;
        for (local_c = *(int *)(pcVar1 + 0x34); local_c != 0; local_c = *(int *)(local_c + 8)) {
          local_10 = local_10 + 1;
        }
        _fputc((uint)local_10,param_1);
        puVar2 = *(undefined4 **)(pcVar1 + 0x34);
        while (puVar2 != (undefined4 *)0x0) {
          iVar3 = FUN_004269a0(*puVar2,param_1);
          local_14 = local_14 + iVar3;
          FUN_0044a460((char *)(puVar2 + 1),4,1,param_1);
        }
      }
    }
    local_18 = local_18 + 1;
  }
  return local_14;
}

