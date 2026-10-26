// rea decompile_all
// name: FUN_0041f80c
// address: 0x41f80c
// body: available
// range: 0x41f80c-0x41f90d inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_0041f80c(int param_1,int param_2,int param_3,int param_4)

{
  short sVar1;
  short sVar2;
  int iVar3;
  int iVar4;
  int *piVar5;
  tagRECT *ptVar6;
  tagRECT local_14;
  
  piVar5 = *(int **)(param_1 + param_2 * 4);
  if (piVar5 != (int *)0x0) {
    sVar1 = *(short *)(piVar5[1] + 6);
    sVar2 = *(short *)(piVar5[1] + 8);
    SetRect(&local_14,(int)sVar1,(int)sVar2,(int)*(short *)(piVar5[1] + 2) + (int)sVar1,
            (int)*(short *)(piVar5[1] + 4) + (int)sVar2);
    if (param_3 == 1) {
      FUN_0040bbb0(sVar1,sVar2,(int)*(short *)(piVar5[1] + 4),0x280);
      FUN_0040bbfd(*piVar5,DAT_00477344,sVar1,sVar2,&local_14.left,0);
    }
    else {
      FUN_00420683(&local_14.left);
    }
    if (param_4 != 0) {
      iVar3 = (int)sVar2;
      iVar4 = (int)sVar1;
      ptVar6 = &local_14;
      piVar5 = (int *)FUN_00418040(DAT_00477344);
      FUN_004181c5(piVar5,&ptVar6->left,iVar4,iVar3);
    }
  }
  return;
}

