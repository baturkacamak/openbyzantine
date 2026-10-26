// rea decompile_all
// name: FUN_00420dc9
// address: 0x420dc9
// body: available
// range: 0x420dc9-0x420f09 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_00420dc9(int param_1,int param_2)

{
  int iVar1;
  int *piVar2;
  char *pcVar3;
  int local_30;
  int local_2c;
  int local_18;
  int local_14;
  undefined4 local_10;
  undefined4 local_c;
  uint local_8;
  
  local_2c = 0x6c;
  local_18 = 0x172;
  local_14 = 0x6c;
  local_10 = 0x230;
  local_c = 0x1ac;
  piVar2 = (int *)FUN_00418040(DAT_00477344);
  if (param_2 != 0) {
    FUN_00420683(&local_18);
  }
  for (local_30 = 0; local_30 < 0x10; local_30 = local_30 + 1) {
    iVar1 = *(int *)(&DAT_0046c800 + local_30 * 4);
    if (iVar1 != -1) {
      local_8 = (uint)(iVar1 == param_1);
      pcVar3 = _strrchr(*(char **)(&DAT_0046c840 + iVar1 * 4),0x3a);
      if (pcVar3 != (char *)0x0) {
        *pcVar3 = '\0';
      }
      FUN_004063e0(*(char **)(&DAT_0046c840 + iVar1 * 4),0x172,local_2c,2,piVar2,local_8,0,1);
      if (pcVar3 != (char *)0x0) {
        *pcVar3 = ':';
      }
      local_2c = local_2c + 0x14;
    }
  }
  if (param_2 != 0) {
    FUN_004181c5(piVar2,&local_18,local_18,local_14);
  }
  return;
}

