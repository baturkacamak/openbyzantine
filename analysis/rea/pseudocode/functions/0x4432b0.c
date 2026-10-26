// rea decompile_all
// name: FUN_004432b0
// address: 0x4432b0
// body: available
// range: 0x4432b0-0x4433e4 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


undefined1 __cdecl FUN_004432b0(int param_1)

{
  short *psVar1;
  undefined4 uVar2;
  int iVar3;
  int iVar4;
  int *piVar5;
  int *piVar6;
  int *piVar7;
  undefined1 local_9;
  int local_8;
  int local_4;
  
  local_9 = 0;
  iVar4 = *(int *)(param_1 + 0x184);
  if ((*(char *)(param_1 + 200) == '\0') || (*(int *)(param_1 + 0x8c) == 0)) {
    return 0;
  }
  if (*(int *)(iVar4 + 0x70) == 0) {
    uVar2 = (*(code *)**(undefined4 **)(param_1 + 4))(param_1,1,*(int *)(param_1 + 0x20) * 0x18);
    *(undefined4 *)(iVar4 + 0x70) = uVar2;
  }
  piVar5 = *(int **)(iVar4 + 0x70);
  iVar4 = *(int *)(param_1 + 0xc4);
  local_4 = 0;
  if (0 < *(int *)(param_1 + 0x20)) {
    local_8 = 0;
    do {
      psVar1 = *(short **)(iVar4 + 0x4c);
      if (psVar1 == (short *)0x0) {
        return 0;
      }
      if ((((*psVar1 == 0) || (psVar1[1] == 0)) || (psVar1[8] == 0)) ||
         (((psVar1[0x10] == 0 || (psVar1[9] == 0)) || (psVar1[2] == 0)))) {
        return 0;
      }
      piVar6 = (int *)(*(int *)(param_1 + 0x8c) + local_8);
      if (*piVar6 < 0) {
        return 0;
      }
      iVar3 = 5;
      piVar7 = piVar5;
      do {
        piVar7 = piVar7 + 1;
        piVar6 = piVar6 + 1;
        *piVar7 = *piVar6;
        if (*piVar6 != 0) {
          local_9 = 1;
        }
        iVar3 = iVar3 + -1;
      } while (iVar3 != 0);
      piVar5 = piVar5 + 6;
      iVar4 = iVar4 + 0x54;
      local_8 = local_8 + 0x100;
      local_4 = local_4 + 1;
    } while (local_4 < *(int *)(param_1 + 0x20));
  }
  return local_9;
}

