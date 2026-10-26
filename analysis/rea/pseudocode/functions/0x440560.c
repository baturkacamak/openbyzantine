// rea decompile_all
// name: FUN_00440560
// address: 0x440560
// body: available
// range: 0x440560-0x4408e0 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


uint __cdecl FUN_00440560(int *param_1)

{
  int iVar1;
  byte bVar2;
  byte bVar3;
  undefined4 *puVar4;
  uint *puVar5;
  uint uVar6;
  byte *pbVar7;
  byte *pbVar8;
  int iVar9;
  byte *pbVar10;
  int iVar11;
  int *piVar12;
  undefined4 *puVar13;
  int local_120;
  uint uStack_11c;
  undefined4 uStack_114;
  uint uStack_110;
  uint uStack_10c;
  uint uStack_108;
  byte bStack_104;
  byte abStack_100 [256];
  
  puVar4 = (undefined4 *)param_1[5];
  pbVar8 = (byte *)*puVar4;
  local_120 = puVar4[1];
  if (local_120 == 0) {
    uVar6 = (*(code *)puVar4[3])(param_1);
    if ((char)uVar6 == '\0') {
      return uVar6 & 0xffffff00;
    }
    pbVar8 = (byte *)*puVar4;
    local_120 = puVar4[1];
  }
  pbVar7 = pbVar8 + 1;
  local_120 = local_120 + -1;
  bVar2 = *pbVar8;
  if (local_120 == 0) {
    uVar6 = (*(code *)puVar4[3])(param_1);
    if ((char)uVar6 == '\0') {
      return uVar6 & 0xffffff00;
    }
    pbVar7 = (byte *)*puVar4;
    local_120 = puVar4[1];
  }
  local_120 = local_120 + -1;
  iVar1 = (uint)bVar2 * 0x100 + (uint)*pbVar7 + -2;
  pbVar8 = pbVar7 + 1;
  do {
    if (iVar1 < 1) {
      *puVar4 = pbVar8;
      puVar4[1] = local_120;
      return CONCAT31((int3)((uint)local_120 >> 8),1);
    }
    if (local_120 == 0) {
      uVar6 = (*(code *)puVar4[3])(param_1);
      if ((char)uVar6 == '\0') {
        return uVar6 & 0xffffff00;
      }
      pbVar8 = (byte *)*puVar4;
      local_120 = puVar4[1];
    }
    pbVar7 = pbVar8 + 1;
    local_120 = local_120 + -1;
    bVar2 = *pbVar8;
    uStack_11c = (uint)bVar2;
    *(undefined4 *)(*param_1 + 0x14) = 0x4f;
    iVar11 = 0;
    iVar9 = 1;
    *(uint *)(*param_1 + 0x18) = uStack_11c;
    (**(code **)(*param_1 + 4))(param_1,1);
    uStack_114 = uStack_114 & 0xffffff00;
    do {
      if (local_120 == 0) {
        uVar6 = (*(code *)puVar4[3])(param_1);
        if ((char)uVar6 == '\0') {
          return uVar6 & 0xffffff00;
        }
        pbVar7 = (byte *)*puVar4;
        local_120 = puVar4[1];
      }
      bVar3 = *pbVar7;
      pbVar7 = pbVar7 + 1;
      *(byte *)((int)&uStack_114 + iVar9) = bVar3;
      iVar9 = iVar9 + 1;
      iVar11 = iVar11 + (uint)bVar3;
      local_120 = local_120 + -1;
    } while (iVar9 < 0x11);
    iVar9 = *param_1;
    *(uint *)(iVar9 + 0x18) = uStack_114 >> 8 & 0xff;
    *(uint *)(iVar9 + 0x1c) = uStack_114 >> 0x10 & 0xff;
    *(uint *)(iVar9 + 0x20) = uStack_114 >> 0x18;
    *(uint *)(iVar9 + 0x24) = uStack_110 & 0xff;
    *(uint *)(iVar9 + 0x28) = uStack_110 >> 8 & 0xff;
    *(uint *)(iVar9 + 0x2c) = uStack_110 >> 0x10 & 0xff;
    *(uint *)(iVar9 + 0x30) = uStack_110 >> 0x18;
    *(uint *)(iVar9 + 0x34) = uStack_10c & 0xff;
    *(undefined4 *)(*param_1 + 0x14) = 0x55;
    (**(code **)(*param_1 + 4))(param_1,2);
    iVar9 = *param_1;
    *(uint *)(iVar9 + 0x18) = uStack_10c >> 8 & 0xff;
    *(uint *)(iVar9 + 0x1c) = uStack_10c >> 0x10 & 0xff;
    *(uint *)(iVar9 + 0x20) = uStack_10c >> 0x18;
    *(uint *)(iVar9 + 0x24) = uStack_108 & 0xff;
    *(uint *)(iVar9 + 0x28) = uStack_108 >> 8 & 0xff;
    *(uint *)(iVar9 + 0x2c) = uStack_108 >> 0x10 & 0xff;
    *(uint *)(iVar9 + 0x30) = uStack_108 >> 0x18;
    *(uint *)(iVar9 + 0x34) = (uint)bStack_104;
    *(undefined4 *)(*param_1 + 0x14) = 0x55;
    (**(code **)(*param_1 + 4))(param_1,2);
    if ((0x100 < iVar11) || (iVar1 + -0x11 < iVar11)) {
      *(undefined4 *)(*param_1 + 0x14) = 0x1c;
      (**(code **)*param_1)(param_1);
    }
    iVar9 = 0;
    if (0 < iVar11) {
      do {
        if (local_120 == 0) {
          uVar6 = (*(code *)puVar4[3])(param_1);
          if ((char)uVar6 == '\0') {
            return uVar6 & 0xffffff00;
          }
          pbVar7 = (byte *)*puVar4;
          local_120 = puVar4[1];
        }
        bVar3 = *pbVar7;
        pbVar7 = pbVar7 + 1;
        abStack_100[iVar9] = bVar3;
        iVar9 = iVar9 + 1;
        local_120 = local_120 + -1;
      } while (iVar9 < iVar11);
    }
    iVar1 = (iVar1 + -0x11) - iVar11;
    if ((bVar2 & 0x10) == 0) {
      iVar9 = uStack_11c + 0x28;
    }
    else {
      iVar9 = uStack_11c + 0x1c;
      uStack_11c = uStack_11c - 0x10;
    }
    piVar12 = param_1 + iVar9;
    if (((int)uStack_11c < 0) || (3 < (int)uStack_11c)) {
      *(undefined4 *)(*param_1 + 0x14) = 0x1d;
      *(uint *)(*param_1 + 0x18) = uStack_11c;
      (**(code **)*param_1)(param_1);
    }
    if (*piVar12 == 0) {
      iVar9 = FUN_00441a90((int)param_1);
      *piVar12 = iVar9;
    }
    puVar5 = (uint *)*piVar12;
    *puVar5 = uStack_114;
    puVar5[1] = uStack_110;
    puVar5[2] = uStack_10c;
    puVar5[3] = uStack_108;
    *(byte *)(puVar5 + 4) = bStack_104;
    pbVar10 = abStack_100;
    puVar13 = (undefined4 *)(*piVar12 + 0x11);
    for (iVar9 = 0x40; pbVar8 = pbVar7, iVar9 != 0; iVar9 = iVar9 + -1) {
      *puVar13 = *(undefined4 *)pbVar10;
      pbVar10 = pbVar10 + 4;
      puVar13 = puVar13 + 1;
    }
  } while( true );
}

