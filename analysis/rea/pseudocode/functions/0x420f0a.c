// rea decompile_all
// name: FUN_00420f0a
// address: 0x420f0a
// body: available
// range: 0x420f0a-0x420fe4 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_00420f0a(int param_1)

{
  int *piVar1;
  tagRECT *ptVar2;
  int iVar3;
  int iVar4;
  char local_11c [260];
  int *local_18;
  tagRECT local_14;
  
  local_18 = (int *)0x0;
  local_14.left = 0;
  local_14.top = 0;
  local_14.right = 0;
  local_14.bottom = 0;
  if (param_1 == -1) {
    SetRect(&local_14,0x77,0x38,0x120,0x10f);
    iVar4 = 0x38;
    iVar3 = 0x77;
    ptVar2 = &local_14;
    piVar1 = (int *)FUN_00418040(DAT_0047a8a4);
    FUN_004181c5(piVar1,&ptVar2->left,iVar3,iVar4);
  }
  else {
    _sprintf(local_11c,s_setupart_portrait_photo_02d_tga_0045fc58,param_1);
    FUN_00416710(local_11c,&local_18,1,(uint *)&local_14.right,(uint *)&local_14.bottom);
    if (local_18 != (int *)0x0) {
      FUN_004181c5(local_18,&local_14.left,0x77,0x38);
      (**(code **)(*local_18 + 8))(local_18);
    }
  }
  return;
}

