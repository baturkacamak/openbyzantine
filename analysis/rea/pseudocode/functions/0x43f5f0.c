// rea decompile_all
// name: FUN_0043f5f0
// address: 0x43f5f0
// body: available
// range: 0x43f5f0-0x43f7b3 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


uint __cdecl FUN_0043f5f0(int *param_1)

{
  int iVar1;
  int iVar2;
  byte bVar3;
  undefined4 *puVar4;
  uint uVar5;
  undefined4 uVar6;
  byte *pbVar7;
  byte *pbVar8;
  int iVar9;
  int local_14;
  int iStack_10;
  byte abStack_c [4];
  char cStack_8;
  byte bStack_7;
  byte bStack_6;
  byte bStack_5;
  byte bStack_4;
  byte bStack_3;
  byte bStack_2;
  byte bStack_1;
  
  puVar4 = (undefined4 *)param_1[5];
  pbVar7 = (byte *)*puVar4;
  iVar9 = puVar4[1];
  if (iVar9 == 0) {
    uVar5 = (*(code *)puVar4[3])(param_1);
    if ((char)uVar5 == '\0') {
      return uVar5 & 0xffffff00;
    }
    pbVar7 = (byte *)*puVar4;
    iVar9 = puVar4[1];
  }
  iVar9 = iVar9 + -1;
  bVar3 = *pbVar7;
  pbVar7 = pbVar7 + 1;
  if (iVar9 == 0) {
    uVar5 = (*(code *)puVar4[3])(param_1);
    if ((char)uVar5 == '\0') {
      return uVar5 & 0xffffff00;
    }
    pbVar7 = (byte *)*puVar4;
    iVar9 = puVar4[1];
  }
  iVar9 = iVar9 + -1;
  pbVar8 = pbVar7 + 1;
  iVar1 = (uint)bVar3 * 0x100 + (uint)*pbVar7;
  local_14 = iVar1 + -2;
  if (local_14 < 0xc) {
    *(undefined4 *)(*param_1 + 0x14) = 0x4d;
    *(int *)(*param_1 + 0x18) = local_14;
  }
  else {
    iStack_10 = 0;
    do {
      if (iVar9 == 0) {
        uVar5 = (*(code *)puVar4[3])(param_1);
        if ((char)uVar5 == '\0') {
          return uVar5 & 0xffffff00;
        }
        pbVar8 = (byte *)*puVar4;
        iVar9 = puVar4[1];
      }
      bVar3 = *pbVar8;
      iVar9 = iVar9 + -1;
      pbVar8 = pbVar8 + 1;
      iVar2 = iStack_10 + 1;
      abStack_c[iStack_10] = bVar3;
      iStack_10 = iVar2;
    } while (iVar2 < 0xc);
    local_14 = iVar1 + -0xe;
    if ((((abStack_c[0] == 0x41) && (abStack_c[1] == 'd')) && (abStack_c[2] == 'o')) &&
       ((abStack_c[3] == 'b' && (cStack_8 == 'e')))) {
      iVar1 = *param_1;
      *(uint *)(iVar1 + 0x18) = (uint)bStack_7 * 0x100 + (uint)bStack_6;
      *(uint *)(iVar1 + 0x1c) = (uint)bStack_5 * 0x100 + (uint)bStack_4;
      *(uint *)(iVar1 + 0x20) = (uint)bStack_3 * 0x100 + (uint)bStack_2;
      *(uint *)(iVar1 + 0x24) = (uint)bStack_1;
      *(undefined4 *)(*param_1 + 0x14) = 0x4b;
      uVar6 = (**(code **)(*param_1 + 4))(param_1,1);
      *(undefined1 *)((int)param_1 + 0x106) = 1;
      *(byte *)((int)param_1 + 0x107) = bStack_1;
      goto LAB_0043f78f;
    }
    *(undefined4 *)(*param_1 + 0x14) = 0x4d;
    *(int *)(*param_1 + 0x18) = iVar1 + -2;
  }
  uVar6 = (**(code **)(*param_1 + 4))(param_1,1);
LAB_0043f78f:
  *puVar4 = pbVar8;
  puVar4[1] = iVar9;
  if (0 < local_14) {
    uVar6 = (**(code **)(param_1[5] + 0x10))(param_1,local_14);
  }
  return CONCAT31((int3)((uint)uVar6 >> 8),1);
}

