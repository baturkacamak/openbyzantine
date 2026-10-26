// rea decompile_all
// name: FUN_00440020
// address: 0x440020
// body: available
// range: 0x440020-0x4403af inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


uint __cdecl FUN_00440020(int *param_1)

{
  byte *pbVar1;
  byte bVar2;
  byte bVar3;
  byte bVar4;
  undefined4 *puVar5;
  uint uVar6;
  uint uVar7;
  uint uVar8;
  undefined4 uVar9;
  int iVar10;
  uint *puVar11;
  byte *local_18;
  int local_14;
  int *piStack_8;
  int iStack_4;
  
  puVar5 = (undefined4 *)param_1[5];
  local_18 = (byte *)*puVar5;
  local_14 = puVar5[1];
  if (*(char *)(param_1[100] + 0x51) == '\0') {
    *(undefined4 *)(*param_1 + 0x14) = 0x3d;
    (**(code **)*param_1)(param_1);
  }
  if (local_14 == 0) {
    uVar6 = (*(code *)puVar5[3])(param_1);
    if ((char)uVar6 == '\0') {
      return uVar6 & 0xffffff00;
    }
    local_18 = (byte *)*puVar5;
    local_14 = puVar5[1];
  }
  local_14 = local_14 + -1;
  bVar2 = *local_18;
  local_18 = local_18 + 1;
  if (local_14 == 0) {
    uVar6 = (*(code *)puVar5[3])(param_1);
    if ((char)uVar6 == '\0') {
      return uVar6 & 0xffffff00;
    }
    local_18 = (byte *)*puVar5;
    local_14 = puVar5[1];
  }
  local_14 = local_14 + -1;
  bVar3 = *local_18;
  local_18 = local_18 + 1;
  if (local_14 == 0) {
    uVar6 = (*(code *)puVar5[3])(param_1);
    if ((char)uVar6 == '\0') {
      return uVar6 & 0xffffff00;
    }
    local_18 = (byte *)*puVar5;
    local_14 = puVar5[1];
  }
  local_14 = local_14 + -1;
  pbVar1 = local_18 + 1;
  bVar4 = *local_18;
  uVar6 = (uint)bVar4;
  if (((uVar6 * 2 + 6 != (uint)bVar2 * 0x100 + (uint)bVar3) || (bVar4 == 0)) || (4 < bVar4)) {
    *(undefined4 *)(*param_1 + 0x14) = 9;
    (**(code **)*param_1)(param_1);
  }
  *(undefined4 *)(*param_1 + 0x14) = 0x66;
  *(uint *)(*param_1 + 0x18) = uVar6;
  (**(code **)(*param_1 + 4))(param_1,1);
  param_1[0x48] = uVar6;
  iStack_4 = 0;
  local_18 = pbVar1;
  if (uVar6 != 0) {
    piStack_8 = param_1 + 0x49;
    do {
      if (local_14 == 0) {
        uVar7 = (*(code *)puVar5[3])(param_1);
        if ((char)uVar7 == '\0') {
          return uVar7 & 0xffffff00;
        }
        local_18 = (byte *)*puVar5;
        local_14 = puVar5[1];
      }
      local_14 = local_14 + -1;
      uVar7 = (uint)*local_18;
      local_18 = local_18 + 1;
      if (local_14 == 0) {
        uVar8 = (*(code *)puVar5[3])(param_1);
        if ((char)uVar8 == '\0') {
          return uVar8 & 0xffffff00;
        }
        local_18 = (byte *)*puVar5;
        local_14 = puVar5[1];
      }
      puVar11 = (uint *)param_1[0x31];
      local_14 = local_14 + -1;
      bVar2 = *local_18;
      local_18 = local_18 + 1;
      iVar10 = 0;
      if (0 < param_1[8]) {
        do {
          if (*puVar11 == uVar7) goto LAB_0044020f;
          iVar10 = iVar10 + 1;
          puVar11 = puVar11 + 0x15;
        } while (iVar10 < param_1[8]);
      }
      *(undefined4 *)(*param_1 + 0x14) = 5;
      *(uint *)(*param_1 + 0x18) = uVar7;
      (**(code **)*param_1)(param_1);
LAB_0044020f:
      *piStack_8 = (int)puVar11;
      puVar11[5] = (uint)(bVar2 >> 4);
      puVar11[6] = bVar2 & 0xf;
      iVar10 = *param_1;
      *(uint *)(iVar10 + 0x18) = uVar7;
      *(uint *)(iVar10 + 0x1c) = puVar11[5];
      *(uint *)(iVar10 + 0x20) = puVar11[6];
      *(undefined4 *)(*param_1 + 0x14) = 0x67;
      (**(code **)(*param_1 + 4))(param_1,1);
      piStack_8 = piStack_8 + 1;
      iStack_4 = iStack_4 + 1;
    } while (iStack_4 < (int)uVar6);
  }
  if (local_14 == 0) {
    uVar6 = (*(code *)puVar5[3])(param_1);
    if ((char)uVar6 == '\0') {
      return uVar6 & 0xffffff00;
    }
    local_18 = (byte *)*puVar5;
    local_14 = puVar5[1];
  }
  local_14 = local_14 + -1;
  param_1[0x5a] = (uint)*local_18;
  local_18 = local_18 + 1;
  if (local_14 == 0) {
    uVar6 = (*(code *)puVar5[3])(param_1);
    if ((char)uVar6 == '\0') {
      return uVar6 & 0xffffff00;
    }
    local_18 = (byte *)*puVar5;
    local_14 = puVar5[1];
  }
  local_14 = local_14 + -1;
  param_1[0x5b] = (uint)*local_18;
  local_18 = local_18 + 1;
  if (local_14 == 0) {
    uVar6 = (*(code *)puVar5[3])(param_1);
    if ((char)uVar6 == '\0') {
      return uVar6 & 0xffffff00;
    }
    local_18 = (byte *)*puVar5;
    local_14 = puVar5[1];
  }
  bVar2 = *local_18;
  param_1[0x5d] = bVar2 & 0xf;
  param_1[0x5c] = (uint)(bVar2 >> 4);
  iVar10 = *param_1;
  *(int *)(iVar10 + 0x18) = param_1[0x5a];
  *(int *)(iVar10 + 0x1c) = param_1[0x5b];
  *(int *)(iVar10 + 0x20) = param_1[0x5c];
  *(int *)(iVar10 + 0x24) = param_1[0x5d];
  *(undefined4 *)(*param_1 + 0x14) = 0x68;
  uVar9 = (**(code **)(*param_1 + 4))(param_1,1);
  *(undefined4 *)(param_1[100] + 0x54) = 0;
  param_1[0x1f] = param_1[0x1f] + 1;
  *puVar5 = local_18 + 1;
  puVar5[1] = local_14 + -1;
  return CONCAT31((int3)((uint)uVar9 >> 8),1);
}

