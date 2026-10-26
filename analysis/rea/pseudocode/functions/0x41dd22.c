// rea decompile_all
// name: FUN_0041dd22
// address: 0x41dd22
// body: available
// range: 0x41dd22-0x41de4a inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


undefined4 __cdecl FUN_0041dd22(int param_1,int param_2)

{
  int iVar1;
  undefined4 uVar2;
  int iVar3;
  int local_1c;
  int local_14 [4];
  
  iVar3 = DAT_0045da78;
  local_14[0] = 3;
  local_14[1] = 4;
  local_14[2] = 5;
  local_14[3] = 6;
  iVar1 = FUN_0040c7b4(local_14[param_1]);
  if (iVar1 == 0) {
    if (param_2 != 0) {
      FUN_0044ba00((uint *)&DAT_00484e30,(uint *)s_quick_save_0045f93c);
      DAT_0045da78 = iVar3;
      iVar3 = FUN_00424dc0(s_savegame_qsave_000_0045f948);
      DAT_0045da78 = local_14[param_1];
      if (iVar3 != 0) {
        return 1;
      }
      DAT_0045f7e0 = 1;
    }
    FUN_00428a16();
    for (local_1c = 0; local_1c < 0x15e; local_1c = local_1c + 1) {
      if (*(int *)(&DAT_00486220 + local_1c * 4) != 0) {
        *(undefined4 *)(*(int *)(&DAT_00486220 + local_1c * 4) + 0x34) = 0;
      }
    }
    FUN_00408f32(0);
    DAT_0046066c = 0;
    DAT_00460668 = 0;
    DAT_004848a8 = 0;
    uVar2 = 0;
  }
  else {
    uVar2 = 1;
  }
  return uVar2;
}

