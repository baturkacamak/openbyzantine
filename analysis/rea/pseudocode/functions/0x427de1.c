// rea decompile_all
// name: FUN_00427de1
// address: 0x427de1
// body: available
// range: 0x427de1-0x427ff4 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_00427de1(int param_1,int param_2)

{
  int iVar1;
  undefined4 uVar2;
  uint local_114 [65];
  int *local_10;
  undefined4 *local_c;
  int *local_8;
  
  local_10 = (int *)0x0;
  local_8 = *(int **)(&DAT_00486220 + param_1 * 4);
  if ((local_8[6] != 0) || (iVar1 = FUN_0041d440(2), iVar1 != 0)) {
    if (*(char *)(*(int *)(&DAT_0047a8e0 + param_1 * 4) + 8) == '\x02') {
      FUN_00428f8c(local_114,(uint *)**(undefined4 **)(&DAT_0047a8e0 + param_1 * 4),
                   (uint *)&DAT_004601d8);
      local_c = FUN_00422b04((LPSTR)local_114,param_1,*(int **)(&DAT_00486220 + param_1 * 4));
      if (local_c == (undefined4 *)0x0) {
        FUN_0042793f(param_1);
      }
      else {
        *(undefined4 **)(&DAT_00485c90 + param_1 * 4) = local_c;
      }
    }
    else {
      if (*(int *)(&DAT_00485c90 + param_1 * 4) != 0) {
        local_10 = *(int **)(*(int *)(&DAT_00485c90 + param_1 * 4) + 8);
      }
      if (local_10 == (int *)0x0) {
        uVar2 = FUN_004283a6(*(int **)(&DAT_00486220 + param_1 * 4),
                             (uint *)**(undefined4 **)(&DAT_0047a8e0 + param_1 * 4));
        *(char *)(*(int *)(&DAT_0047a8e0 + param_1 * 4) + 9) = (char)uVar2;
        if (*(char *)(*(int *)(&DAT_0047a8e0 + param_1 * 4) + 9) != '\0') {
          local_10 = *(int **)(*(int *)(&DAT_00485c90 + param_1 * 4) + 8);
        }
      }
      if (local_10 != (int *)0x0) {
        if (param_2 != 0) {
          (**(code **)(*local_10 + 0x34))(local_10,0);
        }
        FUN_00427d1a(local_8);
        if (*(int *)(*(int *)(&DAT_00486220 + param_1 * 4) + 0x28) != 0) {
          (**(code **)(*local_10 + 0x3c))(local_10,0xffffd8f0);
        }
        if (*(int *)(*(int *)(&DAT_00486220 + param_1 * 4) + 0x14) == 0) {
          (**(code **)(*local_10 + 0x30))(local_10,0,0,0);
        }
        else {
          (**(code **)(*local_10 + 0x30))(local_10,0,0,1);
        }
        if (*(int *)(*(int *)(&DAT_00486220 + param_1 * 4) + 0x28) != 0) {
          FUN_004275ce(param_1,0,0);
        }
      }
    }
  }
  return;
}

