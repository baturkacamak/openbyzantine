// rea decompile_all
// name: FUN_0040186e
// address: 0x40186e
// body: available
// range: 0x40186e-0x401b4b inclusive
// range: 0x401b51-0x401b71 inclusive
// range: 0x401b8a-0x401c20 inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


undefined4 __cdecl FUN_0040186e(undefined4 param_1,int param_2,int param_3)

{
  int iVar1;
  undefined4 extraout_ECX;
  undefined4 extraout_ECX_00;
  undefined4 uVar2;
  undefined4 extraout_EDX;
  undefined4 extraout_EDX_00;
  undefined4 uVar3;
  float10 fVar4;
  float fVar5;
  int *piVar6;
  uint local_21c [65];
  int local_118;
  int local_114;
  uint local_110 [65];
  FILE *local_c;
  uint local_8;
  
  local_118 = 2;
  local_8 = (uint)DAT_00476338;
  FUN_0040df05(0x11);
  _sprintf((char *)local_110,s___2s__s_0045c0b8,(short)param_1,param_1);
  _sprintf((char *)local_21c,s_nodes__s_jpg_0045c0c0,local_110);
  local_c = (FILE *)FUN_0044bc20((char *)local_21c,&DAT_0045c0d0);
  if (local_c == (FILE *)0x0) {
    FUN_0040c624(local_21c,(uint *)s_nodes_0045c0dc,local_110,(uint *)&DAT_0045c0d4);
    local_c = (FILE *)FUN_0044bc20((char *)local_21c,&DAT_0045c0e4);
    if (local_c == (FILE *)0x0) {
      return 1;
    }
  }
  _fclose(local_c);
  if ((param_2 != 0) ||
     (iVar1 = _strcmp((char *)local_21c,&DAT_00468b98), uVar2 = extraout_ECX, uVar3 = extraout_EDX,
     iVar1 != 0)) {
    FUN_0044ba00((uint *)&DAT_00468b98,local_21c);
    iVar1 = FUN_00401d29(&DAT_00468b98);
    uVar2 = extraout_ECX_00;
    uVar3 = extraout_EDX_00;
    if (iVar1 != 0) {
      return 1;
    }
  }
  FUN_00401ddf(uVar2,uVar3);
  FUN_0040d9f7();
  if (param_3 != 100) {
    if (param_3 != 0x6a) {
      FUN_00418bcf((int)DAT_0047a8a4);
    }
    local_118 = 0;
    iVar1 = FUN_0043be09(DAT_0045c040,DAT_00468ca0,DAT_0046487c,DAT_004654d8,1,&local_114);
    if ((iVar1 != 0) && (local_114 != 0)) {
      FUN_0042382b((int)DAT_00464880);
    }
    FUN_0042423d(DAT_00464880,SUB84((double)DAT_004654dc,0),
                 (int)((ulonglong)(double)DAT_004654dc >> 0x20),SUB84((double)DAT_00468b90,0),
                 (int)((ulonglong)(double)DAT_00468b90 >> 0x20),0,0x3ff00000);
    if (DAT_00464878 == 0) {
      piVar6 = DAT_00486888;
      fVar4 = (float10)FUN_00402e00();
      fVar5 = (float)fVar4;
      fVar4 = (float10)FUN_00402e70();
      FUN_004176d8((float)(fVar4 + (float10)180.0),fVar5,piVar6);
    }
    switch(param_3) {
    case 0x65:
      FUN_0042be07(DAT_0047a8a4,DAT_0046e82c,(int)DAT_00477344,2);
      break;
    case 0x66:
      FUN_0042bce1((int)DAT_0047a8a4,(int)DAT_0046e82c);
      break;
    case 0x67:
      FUN_00419cf6((int)DAT_0047a8a4,(int)DAT_0046e82c,DAT_00477344);
      break;
    case 0x68:
      FUN_0041917b();
      FUN_0041920f(DAT_0047a8a4,(int)DAT_00477344,DAT_0046e82c);
      break;
    case 0x69:
      break;
    case 0x6a:
      FUN_00419a51((int)DAT_0046e82c,DAT_00477344);
    }
    DAT_00468b90 = 0;
    DAT_004654dc = 0;
  }
  FUN_0042b29f((int)DAT_0046e82c,0,0x3200);
  FUN_00401ee6(SUB84((double)DAT_004654dc,0),(int)((ulonglong)(double)DAT_004654dc >> 0x20),
               SUB84((double)DAT_00468b90,0),(int)((ulonglong)(double)DAT_00468b90 >> 0x20),
               local_118);
  if (local_8 < 0x20) {
    FUN_0040df05(0);
  }
  else {
    FUN_0040df05((ushort)local_8);
  }
  return 0;
}

