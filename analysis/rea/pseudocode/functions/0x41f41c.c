// rea decompile_all
// name: FUN_0041f41c
// address: 0x41f41c
// body: available
// range: 0x41f41c-0x41f594 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void FUN_0041f41c(void)

{
  int iVar1;
  int iVar2;
  char *pcVar3;
  undefined4 uVar4;
  uint local_16c [18];
  int local_124;
  char local_120 [260];
  int local_1c;
  FILE *local_18;
  char local_14;
  char local_10 [4];
  int local_c;
  int local_8;
  
  local_c = DAT_0045f7cc * 10;
  iVar1 = local_c + 10;
  FUN_0041fde8();
  local_8 = 0;
  for (local_1c = local_c; (local_1c < iVar1 && ((&DAT_0046c660)[local_1c] != 0));
      local_1c = local_1c + 1) {
    _sprintf(local_120,s_savegame__s_0045fa94,(&DAT_0046c660)[local_1c]);
    local_18 = (FILE *)FUN_0044bc20(local_120,&DAT_0045faa0);
    if (local_18 == (FILE *)0x0) {
      return;
    }
    __fread_lk(local_10,4,1,local_18);
    do {
      iVar2 = _fgetc(local_18);
      local_14 = (char)iVar2;
    } while (local_14 != '\0');
    local_124 = 0;
    do {
      iVar2 = _fgetc(local_18);
      *(char *)((int)local_16c + local_124) = (char)iVar2;
      iVar2 = local_124;
      local_124 = local_124 + 1;
    } while (*(char *)((int)local_16c + iVar2) != '\0');
    _fclose(local_18);
    pcVar3 = FUN_0044c580(local_16c);
    uVar4 = FUN_0044c2f0((SIZE_T)(pcVar3 + 1));
    *(undefined4 *)(&DAT_0046c5f8 + local_8 * 4) = uVar4;
    FUN_0044ba00(*(uint **)(&DAT_0046c5f8 + local_8 * 4),local_16c);
    local_8 = local_8 + 1;
  }
  return;
}

