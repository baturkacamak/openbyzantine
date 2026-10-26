// rea decompile_all
// name: FUN_004177c7
// address: 0x4177c7
// body: available
// range: 0x4177c7-0x4179ab inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 __cdecl FUN_004177c7(char *param_1)

{
  undefined4 uVar1;
  char *pcVar2;
  int *piVar3;
  DWORD DVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int local_c;
  char *local_8;
  
  if (param_1 == (char *)0x0) {
    DAT_004603d4 = 0xffffffff;
    DAT_0045f23c = DAT_00487508 * 2 + 0x14a;
    DAT_0045f22c = 0;
    DAT_0045f230 = 1;
    DAT_0045f234 = 1;
    uVar1 = 0;
  }
  else if (DAT_0045f210 == 0) {
    DAT_0045f204 = 1;
    uVar1 = 1;
  }
  else {
    if (DAT_0045f234 != 0) {
      DAT_0045f234 = 0;
      _DAT_0045f238 = timeGetTime();
    }
    for (local_c = 0; local_c < DAT_0045f22c; local_c = local_c + 1) {
      pcVar2 = FUN_0044c520(param_1,'\n');
      param_1 = pcVar2 + 1;
    }
    if (*param_1 == '\0') {
      uVar1 = 1;
    }
    else {
      local_8 = param_1;
      for (local_c = 0; local_c < DAT_0045f230; local_c = local_c + 1) {
        local_8 = FUN_0044c520(local_8,'\n');
        local_8 = local_8 + 1;
      }
      if (DAT_0045c060 == 0) {
        DAT_0045f204 = 1;
      }
      local_8[-1] = '\0';
      iVar7 = 3;
      iVar6 = 3;
      iVar5 = 0;
      piVar3 = (int *)FUN_00418040(DAT_0046e82c);
      FUN_004063e0(param_1,5,DAT_0045f23c,1,piVar3,iVar5,iVar6,iVar7);
      local_8[-1] = '\n';
      DVar4 = timeGetTime();
      if (1000 < DVar4 - _DAT_0045f238) {
        if (*local_8 == '\0') {
          DAT_0045f230 = DAT_0045f230 + -1;
        }
        _DAT_0045f238 = DVar4;
        if (DAT_0045f23c < 0x14b) {
          DAT_0045f22c = DAT_0045f22c + 1;
        }
        else {
          DAT_0045f23c = DAT_0045f23c - DAT_00487508;
          DAT_0045f230 = DAT_0045f230 + 1;
        }
      }
      uVar1 = 0;
    }
  }
  return uVar1;
}

