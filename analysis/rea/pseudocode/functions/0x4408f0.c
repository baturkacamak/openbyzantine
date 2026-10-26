// rea decompile_all
// name: FUN_004408f0
// address: 0x4408f0
// body: available
// range: 0x4408f0-0x440b8e inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


uint __cdecl FUN_004408f0(int *param_1)

{
  byte bVar1;
  undefined4 *puVar2;
  int iVar3;
  uint uVar4;
  int iVar5;
  int iVar6;
  int *piVar7;
  byte *pbVar8;
  int iVar9;
  ushort *puVar10;
  int iStack_10;
  int *piStack_8;
  
  puVar2 = (undefined4 *)param_1[5];
  pbVar8 = (byte *)*puVar2;
  iVar9 = puVar2[1];
  if (iVar9 == 0) {
    uVar4 = (*(code *)puVar2[3])(param_1);
    if ((char)uVar4 == '\0') {
      return uVar4 & 0xffffff00;
    }
    pbVar8 = (byte *)*puVar2;
    iVar9 = puVar2[1];
  }
  iVar9 = iVar9 + -1;
  bVar1 = *pbVar8;
  pbVar8 = pbVar8 + 1;
  if (iVar9 == 0) {
    uVar4 = (*(code *)puVar2[3])(param_1);
    if ((char)uVar4 == '\0') {
      return uVar4 & 0xffffff00;
    }
    pbVar8 = (byte *)*puVar2;
    iVar9 = puVar2[1];
  }
  iVar9 = iVar9 + -1;
  piVar7 = (int *)(uint)*pbVar8;
  pbVar8 = pbVar8 + 1;
  iVar6 = (int)piVar7 + (uint)bVar1 * 0x100 + -2;
  do {
    iVar3 = iVar6;
    if (iVar3 < 1) {
      *puVar2 = pbVar8;
      puVar2[1] = iVar9;
      return CONCAT31((int3)((uint)piVar7 >> 8),1);
    }
    if (iVar9 == 0) {
      uVar4 = (*(code *)puVar2[3])(param_1);
      if ((char)uVar4 == '\0') {
        return uVar4 & 0xffffff00;
      }
      pbVar8 = (byte *)*puVar2;
      iVar9 = puVar2[1];
    }
    iVar9 = iVar9 + -1;
    bVar1 = *pbVar8;
    pbVar8 = pbVar8 + 1;
    uVar4 = bVar1 & 0xf;
    iVar5 = (int)(uint)bVar1 >> 4;
    *(undefined4 *)(*param_1 + 0x14) = 0x50;
    *(uint *)(*param_1 + 0x18) = uVar4;
    *(int *)(*param_1 + 0x1c) = iVar5;
    (**(code **)(*param_1 + 4))(param_1,1);
    if (3 < uVar4) {
      *(undefined4 *)(*param_1 + 0x14) = 0x1e;
      *(uint *)(*param_1 + 0x18) = uVar4;
      (**(code **)*param_1)(param_1);
    }
    piVar7 = param_1 + uVar4 + 0x24;
    if (*piVar7 == 0) {
      iVar6 = FUN_00441a70((int)param_1);
      *piVar7 = iVar6;
    }
    piStack_8 = (int *)&DAT_0045b3b8;
    puVar10 = (ushort *)*piVar7;
    do {
      if (iVar5 == 0) {
        if (iVar9 == 0) {
          uVar4 = (*(code *)puVar2[3])(param_1);
          if ((char)uVar4 == '\0') {
            return uVar4 & 0xffffff00;
          }
          pbVar8 = (byte *)*puVar2;
          iVar9 = puVar2[1];
        }
        iStack_10._0_2_ = (ushort)*pbVar8;
      }
      else {
        if (iVar9 == 0) {
          uVar4 = (*(code *)puVar2[3])(param_1);
          if ((char)uVar4 == '\0') {
            return uVar4 & 0xffffff00;
          }
          pbVar8 = (byte *)*puVar2;
          iVar9 = puVar2[1];
        }
        iVar9 = iVar9 + -1;
        bVar1 = *pbVar8;
        pbVar8 = pbVar8 + 1;
        if (iVar9 == 0) {
          uVar4 = (*(code *)puVar2[3])(param_1);
          if ((char)uVar4 == '\0') {
            return uVar4 & 0xffffff00;
          }
          pbVar8 = (byte *)*puVar2;
          iVar9 = puVar2[1];
        }
        iStack_10._0_2_ = (ushort)bVar1 * 0x100 + (ushort)*pbVar8;
      }
      iVar9 = iVar9 + -1;
      pbVar8 = pbVar8 + 1;
      piVar7 = piStack_8 + 1;
      puVar10[*piStack_8] = (ushort)iStack_10;
      piStack_8 = piVar7;
    } while (piVar7 < &DAT_0045b4b8);
    piVar7 = param_1;
    if (1 < *(int *)(*param_1 + 0x68)) {
      iStack_10 = 8;
      do {
        iVar6 = *param_1;
        *(uint *)(iVar6 + 0x18) = (uint)*puVar10;
        *(uint *)(iVar6 + 0x1c) = (uint)puVar10[1];
        *(uint *)(iVar6 + 0x20) = (uint)puVar10[2];
        *(uint *)(iVar6 + 0x24) = (uint)puVar10[3];
        *(uint *)(iVar6 + 0x28) = (uint)puVar10[4];
        *(uint *)(iVar6 + 0x2c) = (uint)puVar10[5];
        *(uint *)(iVar6 + 0x30) = (uint)puVar10[6];
        *(uint *)(iVar6 + 0x34) = (uint)puVar10[7];
        *(undefined4 *)(*param_1 + 0x14) = 0x5c;
        piVar7 = (int *)(**(code **)(*param_1 + 4))(param_1,2);
        iStack_10 = iStack_10 + -1;
        puVar10 = puVar10 + 8;
      } while (iStack_10 != 0);
    }
    iVar6 = iVar3 + -0x41;
    if (iVar5 != 0) {
      iVar6 = iVar3 + -0x81;
    }
  } while( true );
}

