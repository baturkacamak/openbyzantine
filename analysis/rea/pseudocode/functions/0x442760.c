// rea decompile_all
// name: FUN_00442760
// address: 0x442760
// body: available
// range: 0x442760-0x44289c inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_00442760(int param_1,undefined4 param_2,uint *param_3,uint param_4)

{
  int iVar1;
  int iVar2;
  
  iVar1 = *(int *)(param_1 + 0x180);
  if (*(char *)(iVar1 + 0x30) == '\0') {
    iVar2 = (**(code **)(*(int *)(param_1 + 0x184) + 0xc))
                      (param_1,*(undefined4 *)(iVar1 + 0x38 + *(int *)(iVar1 + 0x40) * 4));
    if (iVar2 == 0) {
      return;
    }
    *(undefined1 *)(iVar1 + 0x30) = 1;
    *(int *)(iVar1 + 0x4c) = *(int *)(iVar1 + 0x4c) + 1;
  }
  iVar2 = *(int *)(iVar1 + 0x44);
  if (iVar2 != 0) {
    if (iVar2 == 1) goto LAB_00442838;
    if (iVar2 != 2) {
      return;
    }
    (**(code **)(*(int *)(param_1 + 0x188) + 4))
              (param_1,*(undefined4 *)(iVar1 + 0x38 + *(int *)(iVar1 + 0x40) * 4),iVar1 + 0x34,
               *(undefined4 *)(iVar1 + 0x48),param_2,param_3,param_4);
    if (*(uint *)(iVar1 + 0x34) < *(uint *)(iVar1 + 0x48)) {
      return;
    }
    *(undefined4 *)(iVar1 + 0x44) = 0;
    if (param_4 <= *param_3) {
      return;
    }
  }
  *(undefined4 *)(iVar1 + 0x34) = 0;
  *(int *)(iVar1 + 0x48) = *(int *)(param_1 + 0x114) + -1;
  if (*(int *)(param_1 + 0x118) == *(int *)(iVar1 + 0x4c)) {
    FUN_004429c0(param_1);
  }
  *(undefined4 *)(iVar1 + 0x44) = 1;
LAB_00442838:
  (**(code **)(*(int *)(param_1 + 0x188) + 4))
            (param_1,*(undefined4 *)(iVar1 + 0x38 + *(int *)(iVar1 + 0x40) * 4),iVar1 + 0x34,
             *(undefined4 *)(iVar1 + 0x48),param_2,param_3,param_4);
  if (*(uint *)(iVar1 + 0x48) <= *(uint *)(iVar1 + 0x34)) {
    if (*(int *)(iVar1 + 0x4c) == 1) {
      FUN_004428a0(param_1);
    }
    *(uint *)(iVar1 + 0x40) = *(uint *)(iVar1 + 0x40) ^ 1;
    *(undefined1 *)(iVar1 + 0x30) = 0;
    *(int *)(iVar1 + 0x34) = *(int *)(param_1 + 0x114) + 1;
    *(int *)(iVar1 + 0x48) = *(int *)(param_1 + 0x114) + 2;
    *(undefined4 *)(iVar1 + 0x44) = 2;
  }
  return;
}

