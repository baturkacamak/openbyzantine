// rea decompile_all
// name: FUN_0041f065
// address: 0x41f065
// body: available
// range: 0x41f065-0x41f1ab inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_0041f065(int param_1)

{
  int iVar1;
  undefined4 uVar2;
  uint local_18 [2];
  FILE *local_10;
  int local_8;
  
  FUN_0041fd8a();
  DAT_0046c884 = 0;
  local_10 = (FILE *)FUN_0044bc20(s_savegame_autosave_btb_0045fa24,&DAT_0045fa20);
  if (local_10 != (FILE *)0x0) {
    _fclose(local_10);
    uVar2 = FUN_0044c2f0(0xd);
    (&DAT_0046c660)[DAT_0046c884] = uVar2;
    iVar1 = DAT_0046c884;
    DAT_0046c884 = DAT_0046c884 + 1;
    FUN_0044ba00((uint *)(&DAT_0046c660)[iVar1],(uint *)s_autosave_btb_0045fa3c);
  }
  local_10 = (FILE *)FUN_0044bc20(PTR_s_savegame_btb_sav_0045f480,&DAT_0045fa4c);
  if (local_10 != (FILE *)0x0) {
    do {
      if (((local_10->_flag & 0x10) != 0) ||
         (local_8 = FUN_0044d340(local_10,&DAT_0045fa50), local_8 != 1)) break;
      uVar2 = FUN_0044c2f0(8);
      (&DAT_0046c660)[DAT_0046c884] = uVar2;
      iVar1 = DAT_0046c884;
      DAT_0046c884 = DAT_0046c884 + 1;
      FUN_0044ba00((uint *)(&DAT_0046c660)[iVar1],local_18);
    } while (DAT_0046c884 != 100);
    _fclose(local_10);
  }
  if (param_1 != 0) {
    FUN_0041f41c();
  }
  return;
}

