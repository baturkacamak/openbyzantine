// rea decompile_all
// name: FUN_004090c4
// address: 0x4090c4
// body: available
// range: 0x4090c4-0x4091e4 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_004090c4(int param_1,int param_2)

{
  short sVar1;
  short sVar2;
  int xLeft;
  int yTop;
  int *local_20;
  tagRECT local_14;
  
  if (param_1 < 5) {
    local_20 = *(int **)(&DAT_00468ce8 + param_1 * 4);
  }
  else {
    local_20 = *(int **)(&DAT_00468d60 + (param_1 + -5) * 4);
  }
  if (local_20 != (int *)0x0) {
    sVar1 = *(short *)(local_20[1] + 6);
    xLeft = (int)sVar1;
    sVar2 = *(short *)(local_20[1] + 8);
    yTop = (int)sVar2;
    SetRect(&local_14,xLeft,yTop,*(short *)(local_20[1] + 2) + xLeft,
            *(short *)(local_20[1] + 4) + yTop);
    if (param_2 == 0) {
      (**(code **)(*DAT_00486888 + 0x1c))(DAT_00486888,xLeft,yTop,DAT_0045d1b8,&local_14,0);
    }
    else {
      FUN_0040bbb0(sVar1,sVar2,(int)*(short *)(local_20[1] + 4),0x280);
      FUN_0040bbfd(*local_20,DAT_0046e82c,sVar1,sVar2,&local_14.left,0);
    }
    FUN_004181c5(DAT_00486888,&local_14.left,xLeft,yTop);
  }
  return;
}

