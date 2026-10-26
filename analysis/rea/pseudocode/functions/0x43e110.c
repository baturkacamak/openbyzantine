// rea decompile_all
// name: FUN_0043e110
// address: 0x43e110
// body: available
// range: 0x43e110-0x43e297 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_0043e110(int *param_1)

{
  int iVar1;
  int *piVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  undefined4 uVar6;
  
  iVar1 = param_1[8];
  if (iVar1 == 1) {
    param_1[9] = 1;
    param_1[10] = 1;
    goto LAB_0043e24f;
  }
  if (iVar1 != 3) {
    if (iVar1 == 4) {
      if ((*(char *)((int)param_1 + 0x106) == '\0') || (*(char *)((int)param_1 + 0x107) == '\0')) {
        param_1[9] = 4;
      }
      else {
        if (*(char *)((int)param_1 + 0x107) != '\x02') {
          *(undefined4 *)(*param_1 + 0x14) = 0x6e;
          *(uint *)(*param_1 + 0x18) = (uint)*(byte *)((int)param_1 + 0x107);
          (**(code **)(*param_1 + 4))(param_1,0xffffffff);
        }
        param_1[9] = 5;
      }
      param_1[10] = 4;
    }
    else {
      param_1[9] = 0;
      param_1[10] = 0;
    }
    goto LAB_0043e24f;
  }
  if ((char)param_1[0x40] == '\0') {
    if (*(char *)((int)param_1 + 0x106) == '\0') {
      piVar2 = (int *)param_1[0x31];
      iVar1 = *piVar2;
      iVar3 = piVar2[0x15];
      iVar4 = piVar2[0x2a];
      if (((iVar1 == 1) && (iVar3 == 2)) && (iVar4 == 3)) goto LAB_0043e1ed;
      if (((iVar1 != 0x52) || (iVar3 != 0x47)) || (iVar4 != 0x42)) {
        iVar5 = *param_1;
        uVar6 = 1;
        *(int *)(iVar5 + 0x18) = iVar1;
        *(int *)(iVar5 + 0x1c) = iVar3;
        *(int *)(iVar5 + 0x20) = iVar4;
        *(undefined4 *)(*param_1 + 0x14) = 0x6b;
        goto LAB_0043e1e4;
      }
      param_1[9] = 2;
    }
    else {
      if (*(char *)((int)param_1 + 0x107) != '\0') {
        if (*(char *)((int)param_1 + 0x107) != '\x01') {
          *(undefined4 *)(*param_1 + 0x14) = 0x6e;
          uVar6 = 0xffffffff;
          *(uint *)(*param_1 + 0x18) = (uint)*(byte *)((int)param_1 + 0x107);
LAB_0043e1e4:
          (**(code **)(*param_1 + 4))(param_1,uVar6);
        }
        goto LAB_0043e1ed;
      }
      param_1[9] = 2;
    }
  }
  else {
LAB_0043e1ed:
    param_1[9] = 3;
  }
  param_1[10] = 2;
LAB_0043e24f:
  param_1[0xb] = 1;
  param_1[0xc] = 1;
  param_1[0xe] = 0;
  *(undefined1 *)(param_1 + 0x10) = 0;
  *(undefined1 *)((int)param_1 + 0x41) = 0;
  param_1[0x11] = 0;
  *(undefined1 *)(param_1 + 0x12) = 1;
  *(undefined1 *)((int)param_1 + 0x49) = 1;
  *(undefined1 *)((int)param_1 + 0x4a) = 0;
  *(undefined1 *)(param_1 + 0x14) = 1;
  param_1[0x1d] = 0;
  *(undefined1 *)(param_1 + 0x16) = 0;
  *(undefined1 *)((int)param_1 + 0x59) = 0;
  *(undefined1 *)((int)param_1 + 0x5a) = 0;
  param_1[0xf] = 0x3ff00000;
  param_1[0x13] = 2;
  param_1[0x15] = 0x100;
  return;
}

