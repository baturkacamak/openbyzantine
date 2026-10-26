// rea decompile_all
// name: FUN_0043eb70
// address: 0x43eb70
// body: available
// range: 0x43eb70-0x43ecfb inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


uint __cdecl FUN_0043eb70(int *param_1)

{
  uint uVar1;
  int iVar2;
  int iVar3;
  uint uVar4;
  uint uVar5;
  
  iVar3 = param_1[0x48];
  if (iVar3 == 1) {
    iVar3 = param_1[0x49];
    param_1[0x4d] = *(int *)(iVar3 + 0x1c);
    param_1[0x4e] = *(int *)(iVar3 + 0x20);
    *(undefined4 *)(iVar3 + 0x34) = 1;
    *(undefined4 *)(iVar3 + 0x38) = 1;
    *(undefined4 *)(iVar3 + 0x3c) = 1;
    *(undefined4 *)(iVar3 + 0x40) = *(undefined4 *)(iVar3 + 0x24);
    *(undefined4 *)(iVar3 + 0x44) = 1;
    uVar4 = *(uint *)(iVar3 + 0xc);
    uVar1 = *(uint *)(iVar3 + 0x20);
    uVar5 = uVar1 % uVar4;
    if (uVar5 == 0) {
      uVar5 = uVar4;
    }
    *(uint *)(iVar3 + 0x48) = uVar5;
    param_1[0x4f] = 1;
    param_1[0x50] = 0;
    return uVar1 / uVar4;
  }
  if ((iVar3 < 1) || (4 < iVar3)) {
    *(undefined4 *)(*param_1 + 0x14) = 0x18;
    *(int *)(*param_1 + 0x18) = param_1[0x48];
    *(undefined4 *)(*param_1 + 0x1c) = 4;
    (**(code **)*param_1)(param_1);
  }
  iVar3 = FUN_00442220(param_1[6],param_1[0x43] << 3);
  param_1[0x4d] = iVar3;
  iVar3 = 0;
  uVar4 = FUN_00442220(param_1[7],param_1[0x44] << 3);
  param_1[0x4e] = uVar4;
  param_1[0x4f] = 0;
  if (0 < param_1[0x48]) {
    do {
      iVar2 = param_1[iVar3 + 0x49];
      *(undefined4 *)(iVar2 + 0x34) = *(undefined4 *)(iVar2 + 8);
      *(int *)(iVar2 + 0x38) = *(int *)(iVar2 + 0xc);
      *(int *)(iVar2 + 0x3c) = *(int *)(iVar2 + 0x34) * *(int *)(iVar2 + 0xc);
      *(int *)(iVar2 + 0x40) = *(int *)(iVar2 + 0x24) * *(int *)(iVar2 + 0x34);
      uVar4 = *(uint *)(iVar2 + 0x1c) % *(uint *)(iVar2 + 0x34);
      if (uVar4 == 0) {
        uVar4 = *(uint *)(iVar2 + 0x34);
      }
      *(uint *)(iVar2 + 0x44) = uVar4;
      uVar4 = *(uint *)(iVar2 + 0x20) % *(uint *)(iVar2 + 0x38);
      if (uVar4 == 0) {
        uVar4 = *(uint *)(iVar2 + 0x38);
      }
      *(uint *)(iVar2 + 0x48) = uVar4;
      uVar4 = *(uint *)(iVar2 + 0x3c);
      uVar1 = uVar4;
      if (10 < (int)(param_1[0x4f] + uVar4)) {
        *(undefined4 *)(*param_1 + 0x14) = 0xb;
        (**(code **)*param_1)(param_1);
      }
      for (; 0 < (int)uVar1; uVar1 = uVar1 - 1) {
        uVar4 = param_1[0x4f];
        param_1[uVar4 + 0x50] = iVar3;
        param_1[0x4f] = param_1[0x4f] + 1;
      }
      iVar3 = iVar3 + 1;
    } while (iVar3 < param_1[0x48]);
  }
  return uVar4;
}

