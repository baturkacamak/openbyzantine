// rea decompile_all
// name: FUN_00425218
// address: 0x425218
// body: available
// range: 0x425218-0x425524 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


int __cdecl FUN_00425218(int param_1,FILE *param_2)

{
  int iVar1;
  bool bVar2;
  int iVar3;
  undefined3 extraout_var;
  int local_10;
  int local_8;
  
  local_8 = 0;
  iVar1 = *(int *)(&DAT_00482ad0 + param_1 * 4);
  if (iVar1 == 0) {
    _fputc(0xff,param_2);
  }
  else {
    _fputc(0,param_2);
    _fputs((char *)(iVar1 + 0x1e94),param_2);
    _fputc(0,param_2);
    FUN_0044a460((char *)(iVar1 + 8),2,1,param_2);
    _fputc((uint)*(byte *)(iVar1 + 10),param_2);
    _fputc((uint)*(byte *)(iVar1 + 0xb),param_2);
    _fputc((uint)*(byte *)(iVar1 + 0xc),param_2);
    _fputc((uint)*(byte *)(iVar1 + 0xd),param_2);
    _fputc((uint)*(byte *)(iVar1 + 0xe),param_2);
    FUN_0044a460((char *)(iVar1 + 0xf),1,0x32,param_2);
    FUN_0044a460((char *)(iVar1 + 0x41),1,0x32,param_2);
    FUN_0044a460((char *)(iVar1 + 0x73),1,0x32,param_2);
    for (local_10 = 0; local_10 < 0x32; local_10 = local_10 + 1) {
      FUN_00425525(*(int *)(iVar1 + 0x4c4 + local_10 * 4),param_2);
    }
    for (local_10 = 0; local_10 < 0x32; local_10 = local_10 + 1) {
      FUN_00425525(*(int *)(iVar1 + 0x58c + local_10 * 4),param_2);
    }
    for (local_10 = 0; local_10 < 0x32; local_10 = local_10 + 1) {
      iVar3 = FUN_004255d1(iVar1,*(byte **)(iVar1 + 0xa8 + local_10 * 4),param_2);
      local_8 = local_8 + iVar3;
    }
    for (local_10 = 0; local_10 < 0x32; local_10 = local_10 + 1) {
      iVar3 = FUN_004255d1(iVar1,*(byte **)(iVar1 + 0x170 + local_10 * 4),param_2);
      local_8 = local_8 + iVar3;
    }
    for (local_10 = 0; local_10 < 0x32; local_10 = local_10 + 1) {
      iVar3 = FUN_004255d1(iVar1,*(byte **)(iVar1 + 0x238 + local_10 * 4),param_2);
      local_8 = local_8 + iVar3;
    }
    for (local_10 = 0; local_10 < 0x32; local_10 = local_10 + 1) {
      bVar2 = FUN_0042571c(iVar1,*(int *)(iVar1 + 0x3c8 + local_10 * 4),param_2,'\0',
                           *(byte *)(local_10 + 0x490 + iVar1));
      local_8 = local_8 + CONCAT31(extraout_var,bVar2);
    }
    FUN_0044a460((char *)(iVar1 + 0x490),1,0x32,param_2);
    for (local_10 = 0; local_10 < 0x32; local_10 = local_10 + 1) {
      FUN_00425952(*(byte **)(iVar1 + 0x654 + local_10 * 4),param_2);
    }
  }
  return local_8;
}

