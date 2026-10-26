// rea decompile_all
// name: FUN_00441ab0
// address: 0x441ab0
// body: available
// range: 0x441ab0-0x441cd8 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_00441ab0(int *param_1)

{
  uint uVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iStack_4;
  
  if (param_1[4] != 0xca) {
    *(undefined4 *)(*param_1 + 0x14) = 0x12;
    *(int *)(*param_1 + 0x18) = param_1[4];
    (**(code **)*param_1)(param_1);
  }
  iVar2 = param_1[0xb];
  uVar1 = param_1[0xc];
  if ((uint)(iVar2 * 8) < uVar1 || iVar2 * 8 - uVar1 == 0) {
    iVar2 = FUN_00442220(param_1[6],8);
    param_1[0x17] = iVar2;
    iVar2 = FUN_00442220(param_1[7],8);
    param_1[0x18] = iVar2;
    param_1[0x45] = 1;
  }
  else if ((uint)(iVar2 * 4) < uVar1 || iVar2 * 4 - uVar1 == 0) {
    iVar2 = FUN_00442220(param_1[6],4);
    param_1[0x17] = iVar2;
    iVar2 = FUN_00442220(param_1[7],4);
    param_1[0x18] = iVar2;
    param_1[0x45] = 2;
  }
  else if (uVar1 < (uint)(iVar2 * 2)) {
    param_1[0x17] = param_1[6];
    param_1[0x18] = param_1[7];
    param_1[0x45] = 8;
  }
  else {
    iVar2 = FUN_00442220(param_1[6],2);
    param_1[0x17] = iVar2;
    iVar2 = FUN_00442220(param_1[7],2);
    param_1[0x18] = iVar2;
    param_1[0x45] = 4;
  }
  iStack_4 = 0;
  iVar2 = param_1[0x31];
  if (0 < param_1[8]) {
    do {
      iVar4 = param_1[0x45];
      iVar3 = iVar4;
      if (iVar4 < 8) {
        do {
          if ((param_1[0x43] * iVar4 < iVar3 * *(int *)(iVar2 + 8) * 2) ||
             (param_1[0x44] * iVar4 < *(int *)(iVar2 + 0xc) * iVar3 * 2)) break;
          iVar3 = iVar3 * 2;
        } while (iVar3 < 8);
      }
      *(int *)(iVar2 + 0x24) = iVar3;
      iVar2 = iVar2 + 0x54;
      iStack_4 = iStack_4 + 1;
    } while (iStack_4 < param_1[8]);
  }
  iVar4 = 0;
  iVar2 = param_1[0x31];
  if (0 < param_1[8]) {
    do {
      iVar4 = iVar4 + 1;
      iVar3 = FUN_00442220(*(int *)(iVar2 + 8) * param_1[6] * *(int *)(iVar2 + 0x24),
                           param_1[0x43] << 3);
      *(int *)(iVar2 + 0x28) = iVar3;
      iVar3 = FUN_00442220(*(int *)(iVar2 + 0xc) * param_1[7] * *(int *)(iVar2 + 0x24),
                           param_1[0x44] << 3);
      *(int *)(iVar2 + 0x2c) = iVar3;
      iVar2 = iVar2 + 0x54;
    } while (iVar4 < param_1[8]);
  }
  switch(param_1[10]) {
  case 1:
    param_1[0x19] = 1;
    break;
  case 2:
  case 3:
    param_1[0x19] = 3;
    break;
  case 4:
  case 5:
    param_1[0x19] = 4;
    break;
  default:
    param_1[0x19] = param_1[8];
  }
  if (*(char *)((int)param_1 + 0x4a) == '\0') {
    param_1[0x1a] = param_1[0x19];
  }
  else {
    param_1[0x1a] = 1;
  }
  iVar2 = FUN_00441cf0((int)param_1);
  if ((char)iVar2 == '\0') {
    param_1[0x1b] = 1;
    return;
  }
  param_1[0x1b] = param_1[0x44];
  return;
}

