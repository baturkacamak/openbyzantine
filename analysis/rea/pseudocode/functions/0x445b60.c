// rea decompile_all
// name: FUN_00445b60
// address: 0x445b60
// body: available
// range: 0x445b60-0x445c48 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl
FUN_00445b60(int param_1,int *param_2,int *param_3,undefined4 param_4,int param_5,int *param_6,
            int param_7)

{
  int iVar1;
  uint uVar2;
  int iVar3;
  undefined4 *puVar4;
  uint uVar5;
  int local_4;
  
  iVar1 = *(int *)(param_1 + 0x19c);
  if (*(int *)(param_1 + 0x110) <= *(int *)(iVar1 + 0x5c)) {
    local_4 = 0;
    if (0 < *(int *)(param_1 + 0x20)) {
      iVar3 = *(int *)(param_1 + 0xc4);
      puVar4 = (undefined4 *)(iVar1 + 0x34);
      do {
        (*(code *)*puVar4)(param_1,iVar3,puVar4[0xc] * *param_3 * 4 + *param_2,puVar4 + -10);
        local_4 = local_4 + 1;
        param_2 = param_2 + 1;
        iVar3 = iVar3 + 0x54;
        puVar4 = puVar4 + 1;
      } while (local_4 < *(int *)(param_1 + 0x20));
    }
    *(undefined4 *)(iVar1 + 0x5c) = 0;
  }
  uVar5 = *(int *)(param_1 + 0x110) - *(int *)(iVar1 + 0x5c);
  if (*(uint *)(iVar1 + 0x60) < uVar5) {
    uVar5 = *(uint *)(iVar1 + 0x60);
  }
  uVar2 = param_7 - *param_6;
  if (uVar2 < uVar5) {
    uVar5 = uVar2;
  }
  (**(code **)(*(int *)(param_1 + 0x1a0) + 4))
            (param_1,iVar1 + 0xc,*(int *)(iVar1 + 0x5c),param_5 + *param_6 * 4,uVar5);
  *param_6 = *param_6 + uVar5;
  *(int *)(iVar1 + 0x60) = *(int *)(iVar1 + 0x60) - uVar5;
  iVar3 = *(int *)(iVar1 + 0x5c) + uVar5;
  *(int *)(iVar1 + 0x5c) = iVar3;
  if (*(int *)(param_1 + 0x110) <= iVar3) {
    *param_3 = *param_3 + 1;
  }
  return;
}

