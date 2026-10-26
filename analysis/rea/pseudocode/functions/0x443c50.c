// rea decompile_all
// name: FUN_00443c50
// address: 0x443c50
// body: available
// range: 0x443c50-0x443d4c inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


undefined4 __cdecl FUN_00443c50(undefined4 *param_1,uint param_2,int param_3,int param_4)

{
  uint uVar1;
  int iVar2;
  byte *pbVar3;
  
  pbVar3 = (byte *)*param_1;
  iVar2 = param_1[1];
  for (; param_3 < 0x19; param_3 = param_3 + 8) {
    if (param_1[2] == 0) {
      if (iVar2 == 0) {
        uVar1 = (**(code **)(*(int *)(param_1[5] + 0x14) + 0xc))(param_1[5]);
        if ((char)uVar1 == '\0') {
          return uVar1 & 0xffffff00;
        }
        pbVar3 = (byte *)**(undefined4 **)(param_1[5] + 0x14);
        iVar2 = (*(undefined4 **)(param_1[5] + 0x14))[1];
      }
      iVar2 = iVar2 + -1;
      uVar1 = (uint)*pbVar3;
      pbVar3 = pbVar3 + 1;
      if (uVar1 == 0xff) {
        do {
          if (iVar2 == 0) {
            uVar1 = (**(code **)(*(int *)(param_1[5] + 0x14) + 0xc))(param_1[5]);
            if ((char)uVar1 == '\0') {
              return uVar1 & 0xffffff00;
            }
            pbVar3 = (byte *)**(undefined4 **)(param_1[5] + 0x14);
            iVar2 = (*(undefined4 **)(param_1[5] + 0x14))[1];
          }
          iVar2 = iVar2 + -1;
          uVar1 = (uint)*pbVar3;
          pbVar3 = pbVar3 + 1;
        } while (uVar1 == 0xff);
        if (uVar1 != 0) {
          param_1[2] = uVar1;
          goto LAB_00443cde;
        }
        uVar1 = 0xff;
      }
    }
    else {
LAB_00443cde:
      if (param_4 <= param_3) break;
      if (*(char *)param_1[6] == '\0') {
        *(undefined4 *)(*(int *)param_1[5] + 0x14) = 0x71;
        (**(code **)(*(int *)param_1[5] + 4))((int *)param_1[5],0xffffffff);
        *(undefined1 *)param_1[6] = 1;
      }
      uVar1 = 0;
    }
    param_2 = param_2 << 8 | uVar1;
  }
  *param_1 = pbVar3;
  param_1[1] = iVar2;
  param_1[3] = param_2;
  param_1[4] = param_3;
  return CONCAT31((int3)((uint)param_3 >> 8),1);
}

