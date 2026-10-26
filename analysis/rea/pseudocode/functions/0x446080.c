// rea decompile_all
// name: FUN_00446080
// address: 0x446080
// body: available
// range: 0x446080-0x4461c9 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_00446080(int *param_1)

{
  undefined4 *puVar1;
  int iVar2;
  int iVar3;
  int *piVar4;
  
  puVar1 = (undefined4 *)(**(code **)param_1[1])(param_1,1,0x18);
  param_1[0x68] = (int)puVar1;
  *puVar1 = &DAT_004465b0;
  switch(param_1[9]) {
  case 1:
    if (param_1[8] != 1) {
LAB_004460d8:
      *(undefined4 *)(*param_1 + 0x14) = 8;
      (**(code **)*param_1)(param_1);
    }
    break;
  case 2:
  case 3:
    if (param_1[8] != 3) goto LAB_004460d8;
    break;
  case 4:
  case 5:
    if (param_1[8] != 4) goto LAB_004460d8;
    break;
  default:
    if (param_1[8] < 1) goto LAB_004460d8;
  }
  piVar4 = param_1 + 8;
  iVar2 = param_1[10];
  if (iVar2 == 1) {
    param_1[0x19] = 1;
    if ((param_1[9] == 1) || (param_1[9] == 3)) {
      puVar1[1] = &LAB_00446430;
      iVar2 = 1;
      if (1 < *piVar4) {
        iVar3 = 0x54;
        do {
          iVar3 = iVar3 + 0x54;
          iVar2 = iVar2 + 1;
          *(undefined1 *)(param_1[0x31] + -0x24 + iVar3) = 0;
        } while (iVar2 < *piVar4);
      }
      goto LAB_004461af;
    }
  }
  else if (iVar2 == 2) {
    param_1[0x19] = 3;
    if (param_1[9] == 3) {
      puVar1[1] = &LAB_004462b0;
      FUN_004461e0((int)param_1);
      goto LAB_004461af;
    }
    if (param_1[9] == 2) goto LAB_00446195;
  }
  else if (iVar2 == 4) {
    param_1[0x19] = 4;
    if (param_1[9] == 5) {
      puVar1[1] = &LAB_00446460;
      FUN_004461e0((int)param_1);
      goto LAB_004461af;
    }
    if (param_1[9] == 4) goto LAB_00446195;
  }
  else if (param_1[9] == iVar2) {
    param_1[0x19] = *piVar4;
LAB_00446195:
    puVar1[1] = &LAB_004463c0;
    goto LAB_004461af;
  }
  *(undefined4 *)(*param_1 + 0x14) = 0x19;
  (**(code **)*param_1)(param_1);
LAB_004461af:
  if (*(char *)((int)param_1 + 0x4a) == '\0') {
    param_1[0x1a] = param_1[0x19];
    return;
  }
  param_1[0x1a] = 1;
  return;
}

