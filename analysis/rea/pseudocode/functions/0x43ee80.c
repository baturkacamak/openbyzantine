// rea decompile_all
// name: FUN_0043ee80
// address: 0x43ee80
// body: available
// range: 0x43ee80-0x43f071 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_0043ee80(int *param_1)

{
  int iVar1;
  int iVar2;
  int iVar3;
  
  if ((0xffdc < param_1[7]) || (0xffdc < param_1[6])) {
    *(undefined4 *)(*param_1 + 0x14) = 0x28;
    *(undefined4 *)(*param_1 + 0x18) = 0xffdc;
    (**(code **)*param_1)(param_1);
  }
  if (param_1[0x30] != 8) {
    *(undefined4 *)(*param_1 + 0x14) = 0xd;
    *(int *)(*param_1 + 0x18) = param_1[0x30];
    (**(code **)*param_1)(param_1);
  }
  if (10 < param_1[8]) {
    *(undefined4 *)(*param_1 + 0x14) = 0x18;
    *(int *)(*param_1 + 0x18) = param_1[8];
    *(undefined4 *)(*param_1 + 0x1c) = 10;
    (**(code **)*param_1)(param_1);
  }
  iVar3 = 0;
  iVar2 = param_1[0x31];
  param_1[0x43] = 1;
  param_1[0x44] = 1;
  if (0 < param_1[8]) {
    do {
      if ((((*(int *)(iVar2 + 8) < 1) || (4 < *(int *)(iVar2 + 8))) || (*(int *)(iVar2 + 0xc) < 1))
         || (4 < *(int *)(iVar2 + 0xc))) {
        *(undefined4 *)(*param_1 + 0x14) = 0x10;
        (**(code **)*param_1)(param_1);
      }
      iVar1 = *(int *)(iVar2 + 8);
      if (*(int *)(iVar2 + 8) <= param_1[0x43]) {
        iVar1 = param_1[0x43];
      }
      param_1[0x43] = iVar1;
      iVar1 = *(int *)(iVar2 + 0xc);
      if (*(int *)(iVar2 + 0xc) <= param_1[0x44]) {
        iVar1 = param_1[0x44];
      }
      iVar3 = iVar3 + 1;
      iVar2 = iVar2 + 0x54;
      param_1[0x44] = iVar1;
    } while (iVar3 < param_1[8]);
  }
  iVar3 = 0;
  param_1[0x45] = 8;
  iVar2 = param_1[0x31];
  if (0 < param_1[8]) {
    do {
      *(undefined4 *)(iVar2 + 0x24) = 8;
      iVar3 = iVar3 + 1;
      iVar1 = FUN_00442220(*(int *)(iVar2 + 8) * param_1[6],param_1[0x43] << 3);
      *(int *)(iVar2 + 0x1c) = iVar1;
      iVar1 = FUN_00442220(*(int *)(iVar2 + 0xc) * param_1[7],param_1[0x44] << 3);
      *(int *)(iVar2 + 0x20) = iVar1;
      iVar1 = FUN_00442220(*(int *)(iVar2 + 8) * param_1[6],param_1[0x43]);
      *(int *)(iVar2 + 0x28) = iVar1;
      iVar1 = FUN_00442220(*(int *)(iVar2 + 0xc) * param_1[7],param_1[0x44]);
      *(int *)(iVar2 + 0x2c) = iVar1;
      *(undefined1 *)(iVar2 + 0x30) = 1;
      *(undefined4 *)(iVar2 + 0x4c) = 0;
      iVar2 = iVar2 + 0x54;
    } while (iVar3 < param_1[8]);
  }
  iVar2 = FUN_00442220(param_1[7],param_1[0x44] << 3);
  param_1[0x46] = iVar2;
  if ((param_1[8] <= param_1[0x48]) && ((char)param_1[0x32] == '\0')) {
    *(undefined1 *)(param_1[99] + 0x10) = 0;
    return;
  }
  *(undefined1 *)(param_1[99] + 0x10) = 1;
  return;
}

