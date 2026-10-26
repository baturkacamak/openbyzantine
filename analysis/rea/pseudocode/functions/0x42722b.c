// rea decompile_all
// name: FUN_0042722b
// address: 0x42722b
// body: available
// range: 0x42722b-0x427388 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_0042722b(FILE *param_1)

{
  char *pcVar1;
  int iVar2;
  undefined4 uVar3;
  char *pcVar4;
  int local_10;
  char local_c;
  char *local_8;
  
  local_8 = (char *)0x0;
  __fread_lk((char *)&DAT_0045fd2c,4,1,param_1);
  for (local_10 = 0; local_10 < 0x15e; local_10 = local_10 + 1) {
    iVar2 = _fgetc(param_1);
    if ((char)iVar2 == '\0') {
      uVar3 = FUN_0044c2f0(0x3c);
      *(undefined4 *)(&DAT_00486220 + local_10 * 4) = uVar3;
      __fread_lk(*(char **)(&DAT_00486220 + local_10 * 4),0x3c,1,param_1);
    }
  }
  iVar2 = _fgetc(param_1);
  local_c = (char)iVar2;
  while (local_c != -1) {
    pcVar4 = (char *)FUN_0044c2f0(0x5c);
    __fread_lk(pcVar4,0x5c,1,param_1);
    if ((*(int *)(pcVar4 + 0x30) == 0) && (iVar2 = FUN_0041d440(2), iVar2 == 0)) {
      FUN_0044ac40(pcVar4);
      pcVar1 = DAT_0046019c;
      pcVar4 = local_8;
    }
    else {
      pcVar4[0x54] = '\0';
      pcVar4[0x55] = '\0';
      pcVar4[0x56] = '\0';
      pcVar4[0x57] = '\0';
      pcVar4[0x58] = '\0';
      pcVar4[0x59] = '\0';
      pcVar4[0x5a] = '\0';
      pcVar4[0x5b] = '\0';
      pcVar1 = pcVar4;
      if (local_8 != (char *)0x0) {
        *(char **)(local_8 + 0x58) = pcVar4;
        pcVar1 = DAT_0046019c;
      }
    }
    local_8 = pcVar4;
    DAT_0046019c = pcVar1;
    iVar2 = _fgetc(param_1);
    local_c = (char)iVar2;
  }
  return;
}

