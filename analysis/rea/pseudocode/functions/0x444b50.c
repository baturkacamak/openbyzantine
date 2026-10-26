// rea decompile_all
// name: FUN_00444b50
// address: 0x444b50
// body: available
// range: 0x444b50-0x444ded inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


uint __cdecl FUN_00444b50(int param_1,int *param_2)

{
  int iVar1;
  int iVar2;
  uint uVar3;
  uint uVar4;
  uint uVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  byte local_34;
  int local_2c;
  undefined4 local_28;
  undefined4 local_24;
  undefined4 local_20;
  uint local_1c;
  int local_18;
  int local_14;
  int local_10;
  int local_c;
  undefined4 local_8;
  int local_4;
  
  iVar1 = *(int *)(param_1 + 0x194);
  local_c = *(int *)(param_1 + 0x16c);
  local_8 = *(undefined4 *)(param_1 + 0x174);
  if (((*(int *)(param_1 + 0xfc) != 0) && (*(int *)(iVar1 + 0x28) == 0)) &&
     (uVar3 = FUN_00444ad0(param_1), (char)uVar3 == '\0')) {
    return uVar3 & 0xffffff00;
  }
  if (*(int *)(iVar1 + 0x14) == 0) {
    local_14 = param_1;
    local_28 = **(undefined4 **)(param_1 + 0x14);
    local_24 = (*(undefined4 **)(param_1 + 0x14))[1];
    local_10 = iVar1 + 0x10;
    local_20 = *(undefined4 *)(param_1 + 0x178);
    uVar3 = *(uint *)(iVar1 + 8);
    iVar6 = *(int *)(iVar1 + 0xc);
    local_4 = *param_2;
    iVar8 = *(int *)(param_1 + 0x168);
    iVar2 = *(int *)(iVar1 + 0x3c);
    local_2c = 0;
    if (iVar8 <= local_c) {
      do {
        if (iVar6 < 8) {
          uVar3 = FUN_00443c50(&local_28,uVar3,iVar6,0);
          if ((char)uVar3 == '\0') {
            return uVar3 & 0xffffff00;
          }
          iVar6 = local_18;
          uVar3 = local_1c;
          if (7 < local_18) goto LAB_00444c41;
          iVar7 = 1;
LAB_00444c6b:
          uVar4 = FUN_00443d50(&local_28,uVar3,iVar6,iVar2,iVar7);
          iVar6 = local_18;
          uVar3 = local_1c;
          if ((int)uVar4 < 0) {
            return uVar4 & 0xffffff00;
          }
        }
        else {
LAB_00444c41:
          uVar4 = (int)uVar3 >> ((char)iVar6 - 8U & 0x1f) & 0xff;
          iVar7 = *(int *)(iVar2 + 0xd4 + uVar4 * 4);
          if (iVar7 == 0) {
            iVar7 = 9;
            goto LAB_00444c6b;
          }
          uVar4 = (uint)*(byte *)(uVar4 + 0x4d4 + iVar2);
          iVar6 = iVar6 - iVar7;
        }
        iVar7 = (int)uVar4 >> 4;
        uVar4 = uVar4 & 0xf;
        if (uVar4 == 0) {
          if (iVar7 != 0xf) {
            local_34 = (byte)iVar7;
            local_2c = 1 << (local_34 & 0x1f);
            if (iVar7 != 0) {
              if ((iVar6 < iVar7) &&
                 (uVar4 = FUN_00443c50(&local_28,uVar3,iVar6,iVar7), iVar6 = local_18,
                 uVar3 = local_1c, (char)uVar4 == '\0')) {
                return uVar4 & 0xffffff00;
              }
              iVar6 = iVar6 - iVar7;
              local_2c = local_2c +
                         ((int)uVar3 >> ((byte)iVar6 & 0x1f) & (1 << (local_34 & 0x1f)) - 1U);
            }
            local_2c = local_2c + -1;
            break;
          }
          iVar8 = iVar8 + 0xf;
        }
        else {
          iVar8 = iVar8 + iVar7;
          if ((iVar6 < (int)uVar4) &&
             (uVar5 = FUN_00443c50(&local_28,uVar3,iVar6,uVar4), iVar6 = local_18, uVar3 = local_1c,
             (char)uVar5 == '\0')) {
            return uVar5 & 0xffffff00;
          }
          iVar6 = iVar6 - uVar4;
          uVar5 = (int)uVar3 >> ((byte)iVar6 & 0x1f) & (1 << (sbyte)uVar4) - 1U;
          if ((int)uVar5 < *(int *)(&DAT_0045b578 + uVar4 * 4)) {
            uVar5 = uVar5 + *(int *)(&DAT_0045b5b8 + uVar4 * 4);
          }
          *(short *)(local_4 + *(int *)(&DAT_0045b3b8 + iVar8 * 4) * 2) =
               (short)uVar5 << ((byte)local_8 & 0x1f);
        }
        iVar8 = iVar8 + 1;
      } while (iVar8 <= local_c);
    }
    **(undefined4 **)(param_1 + 0x14) = local_28;
    *(undefined4 *)(*(int *)(param_1 + 0x14) + 4) = local_24;
    *(undefined4 *)(param_1 + 0x178) = local_20;
    *(uint *)(iVar1 + 8) = uVar3;
    *(int *)(iVar1 + 0xc) = iVar6;
  }
  else {
    local_2c = *(int *)(iVar1 + 0x14) + -1;
  }
  *(int *)(iVar1 + 0x14) = local_2c;
  *(int *)(iVar1 + 0x28) = *(int *)(iVar1 + 0x28) + -1;
  return CONCAT31((int3)((uint)local_2c >> 8),1);
}

