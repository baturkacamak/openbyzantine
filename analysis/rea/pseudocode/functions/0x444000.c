// rea decompile_all
// name: FUN_00444000
// address: 0x444000
// body: available
// range: 0x444000-0x4444b1 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


uint __cdecl FUN_00444000(int param_1,undefined4 *param_2)

{
  int iVar1;
  int iVar2;
  uint uVar3;
  uint uVar4;
  uint uVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  undefined4 local_40;
  undefined4 local_3c;
  undefined4 local_38;
  uint local_34;
  int local_30;
  int local_2c;
  int local_28;
  undefined2 *local_24;
  int *local_20;
  undefined4 *local_1c;
  int local_18;
  int *local_14;
  int local_10 [4];
  
  iVar1 = *(int *)(param_1 + 0x194);
  if (((*(int *)(param_1 + 0xfc) != 0) && (*(int *)(iVar1 + 0x24) == 0)) &&
     (uVar3 = FUN_004444c0(param_1), (char)uVar3 == '\0')) {
    return uVar3 & 0xffffff00;
  }
  local_2c = param_1;
  local_40 = **(undefined4 **)(param_1 + 0x14);
  local_28 = iVar1 + 0x10;
  local_3c = (*(undefined4 **)(param_1 + 0x14))[1];
  local_38 = *(undefined4 *)(param_1 + 0x178);
  local_14 = (int *)(iVar1 + 0x14);
  uVar3 = *(uint *)(iVar1 + 8);
  iVar6 = *(int *)(iVar1 + 0xc);
  local_10[0] = *local_14;
  local_10[1] = *(int *)(iVar1 + 0x18);
  local_10[2] = *(int *)(iVar1 + 0x1c);
  local_10[3] = *(int *)(iVar1 + 0x20);
  local_18 = 0;
  if (0 < *(int *)(param_1 + 0x13c)) {
    local_20 = (int *)(param_1 + 0x140);
    local_1c = param_2;
    do {
      local_24 = (undefined2 *)*local_1c;
      iVar10 = *local_20;
      iVar8 = *(int *)(param_1 + 0x124 + iVar10 * 4);
      iVar9 = *(int *)(iVar1 + 0x28 + *(int *)(iVar8 + 0x14) * 4);
      iVar2 = *(int *)(iVar1 + 0x38 + *(int *)(iVar8 + 0x18) * 4);
      if (iVar6 < 8) {
        uVar3 = FUN_00443c50(&local_40,uVar3,iVar6,0);
        if ((char)uVar3 == '\0') {
          return uVar3 & 0xffffff00;
        }
        iVar6 = local_30;
        uVar3 = local_34;
        if (7 < local_30) goto LAB_00444122;
        iVar7 = 1;
LAB_00444154:
        uVar4 = FUN_00443d50(&local_40,uVar3,iVar6,iVar9,iVar7);
        iVar6 = local_30;
        uVar3 = local_34;
        if ((int)uVar4 < 0) {
          return uVar4 & 0xffffff00;
        }
      }
      else {
LAB_00444122:
        uVar4 = (int)uVar3 >> ((char)iVar6 - 8U & 0x1f) & 0xff;
        iVar7 = *(int *)(iVar9 + 0xd4 + uVar4 * 4);
        if (iVar7 == 0) {
          iVar7 = 9;
          goto LAB_00444154;
        }
        uVar4 = (uint)*(byte *)(iVar9 + 0x4d4 + uVar4);
        iVar6 = iVar6 - iVar7;
      }
      uVar5 = 0;
      if (uVar4 != 0) {
        if ((iVar6 < (int)uVar4) &&
           (uVar5 = FUN_00443c50(&local_40,uVar3,iVar6,uVar4), iVar6 = local_30, uVar3 = local_34,
           (char)uVar5 == '\0')) {
          return uVar5 & 0xffffff00;
        }
        iVar6 = iVar6 - uVar4;
        uVar5 = (int)uVar3 >> ((byte)iVar6 & 0x1f) & (1 << ((byte)uVar4 & 0x1f)) - 1U;
        if ((int)uVar5 < *(int *)(&DAT_0045b4f8 + uVar4 * 4)) {
          uVar5 = *(int *)(&DAT_0045b538 + uVar4 * 4) + uVar5;
        }
      }
      if (*(char *)(iVar8 + 0x30) == '\0') {
LAB_00444315:
        iVar10 = 1;
        do {
          if (iVar6 < 8) {
            uVar3 = FUN_00443c50(&local_40,uVar3,iVar6,0);
            if ((char)uVar3 == '\0') {
              return uVar3 & 0xffffff00;
            }
            iVar6 = local_30;
            uVar3 = local_34;
            if (7 < local_30) goto LAB_0044434c;
            iVar8 = 1;
LAB_00444376:
            uVar4 = FUN_00443d50(&local_40,uVar3,iVar6,iVar2,iVar8);
            iVar6 = local_30;
            uVar3 = local_34;
            if ((int)uVar4 < 0) {
              return uVar4 & 0xffffff00;
            }
          }
          else {
LAB_0044434c:
            uVar4 = (int)uVar3 >> ((char)iVar6 - 8U & 0x1f) & 0xff;
            iVar8 = *(int *)(iVar2 + 0xd4 + uVar4 * 4);
            if (iVar8 == 0) {
              iVar8 = 9;
              goto LAB_00444376;
            }
            uVar4 = (uint)*(byte *)(iVar2 + 0x4d4 + uVar4);
            iVar6 = iVar6 - iVar8;
          }
          uVar5 = uVar4 & 0xf;
          if (uVar5 == 0) {
            if ((int)uVar4 >> 4 != 0xf) break;
            iVar10 = iVar10 + 0xf;
          }
          else {
            iVar10 = iVar10 + ((int)uVar4 >> 4);
            if ((iVar6 < (int)uVar5) &&
               (uVar4 = FUN_00443c50(&local_40,uVar3,iVar6,uVar5), iVar6 = local_30,
               uVar3 = local_34, (char)uVar4 == '\0')) {
              return uVar4 & 0xffffff00;
            }
            iVar6 = iVar6 - uVar5;
          }
          iVar10 = iVar10 + 1;
        } while (iVar10 < 0x40);
      }
      else {
        iVar9 = uVar5 + local_10[iVar10];
        *local_24 = (short)iVar9;
        local_10[iVar10] = iVar9;
        if (*(int *)(iVar8 + 0x24) < 2) goto LAB_00444315;
        iVar10 = 1;
        do {
          if (iVar6 < 8) {
            uVar3 = FUN_00443c50(&local_40,uVar3,iVar6,0);
            if ((char)uVar3 == '\0') {
              return uVar3 & 0xffffff00;
            }
            iVar6 = local_30;
            uVar3 = local_34;
            if (7 < local_30) goto LAB_00444239;
            iVar8 = 1;
LAB_00444263:
            uVar4 = FUN_00443d50(&local_40,uVar3,iVar6,iVar2,iVar8);
            iVar6 = local_30;
            uVar3 = local_34;
            if ((int)uVar4 < 0) {
              return uVar4 & 0xffffff00;
            }
          }
          else {
LAB_00444239:
            uVar4 = (int)uVar3 >> ((char)iVar6 - 8U & 0x1f) & 0xff;
            iVar8 = *(int *)(iVar2 + 0xd4 + uVar4 * 4);
            if (iVar8 == 0) {
              iVar8 = 9;
              goto LAB_00444263;
            }
            uVar4 = (uint)*(byte *)(iVar2 + 0x4d4 + uVar4);
            iVar6 = iVar6 - iVar8;
          }
          uVar5 = uVar4 & 0xf;
          if (uVar5 == 0) {
            if ((int)uVar4 >> 4 != 0xf) break;
            iVar10 = iVar10 + 0xf;
          }
          else {
            iVar10 = iVar10 + ((int)uVar4 >> 4);
            if ((iVar6 < (int)uVar5) &&
               (uVar4 = FUN_00443c50(&local_40,uVar3,iVar6,uVar5), iVar6 = local_30,
               uVar3 = local_34, (char)uVar4 == '\0')) {
              return uVar4 & 0xffffff00;
            }
            iVar6 = iVar6 - uVar5;
            uVar4 = (int)uVar3 >> ((byte)iVar6 & 0x1f) & (1 << (sbyte)uVar5) - 1U;
            if ((int)uVar4 < *(int *)(&DAT_0045b4f8 + uVar5 * 4)) {
              uVar4 = uVar4 + *(int *)(&DAT_0045b538 + uVar5 * 4);
            }
            local_24[*(int *)(&DAT_0045b3b8 + iVar10 * 4)] = (short)uVar4;
          }
          iVar10 = iVar10 + 1;
        } while (iVar10 < 0x40);
      }
      local_18 = local_18 + 1;
      local_20 = local_20 + 1;
      local_1c = local_1c + 1;
    } while (local_18 < *(int *)(param_1 + 0x13c));
  }
  **(undefined4 **)(param_1 + 0x14) = local_40;
  *(undefined4 *)(*(int *)(param_1 + 0x14) + 4) = local_3c;
  *(undefined4 *)(param_1 + 0x178) = local_38;
  *(uint *)(iVar1 + 8) = uVar3;
  *(int *)(iVar1 + 0xc) = iVar6;
  *local_14 = local_10[0];
  local_14[1] = local_10[1];
  local_14[2] = local_10[2];
  local_14[3] = local_10[3];
  *(int *)(iVar1 + 0x24) = *(int *)(iVar1 + 0x24) + -1;
  return CONCAT31((int3)((uint)local_10[3] >> 8),1);
}

