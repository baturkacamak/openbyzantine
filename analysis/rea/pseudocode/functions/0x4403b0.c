// rea decompile_all
// name: FUN_004403b0
// address: 0x4403b0
// body: available
// range: 0x4403b0-0x440556 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


uint __cdecl FUN_004403b0(int *param_1)

{
  int iVar1;
  byte bVar2;
  undefined4 *puVar3;
  byte bVar4;
  uint uVar5;
  uint uVar6;
  byte *pbVar7;
  byte *pbVar8;
  int local_c;
  
  puVar3 = (undefined4 *)param_1[5];
  pbVar8 = (byte *)*puVar3;
  local_c = puVar3[1];
  if (local_c == 0) {
    uVar5 = (*(code *)puVar3[3])(param_1);
    if ((char)uVar5 == '\0') {
      return uVar5 & 0xffffff00;
    }
    pbVar8 = (byte *)*puVar3;
    local_c = puVar3[1];
  }
  pbVar7 = pbVar8 + 1;
  local_c = local_c + -1;
  bVar4 = *pbVar8;
  if (local_c == 0) {
    uVar5 = (*(code *)puVar3[3])(param_1);
    if ((char)uVar5 == '\0') {
      return uVar5 & 0xffffff00;
    }
    pbVar7 = (byte *)*puVar3;
    local_c = puVar3[1];
  }
  iVar1 = (uint)bVar4 * 0x100 + (uint)*pbVar7;
  do {
    iVar1 = iVar1 + -2;
    local_c = local_c + -1;
    pbVar8 = pbVar7 + 1;
    if (iVar1 < 1) {
      *puVar3 = pbVar8;
      puVar3[1] = local_c;
      return CONCAT31((int3)((uint)local_c >> 8),1);
    }
    if (local_c == 0) {
      uVar5 = (*(code *)puVar3[3])(param_1);
      if ((char)uVar5 == '\0') {
        return uVar5 & 0xffffff00;
      }
      pbVar8 = (byte *)*puVar3;
      local_c = puVar3[1];
    }
    pbVar7 = pbVar8 + 1;
    local_c = local_c + -1;
    bVar4 = *pbVar8;
    uVar5 = (uint)bVar4;
    if (local_c == 0) {
      uVar6 = (*(code *)puVar3[3])(param_1);
      if ((char)uVar6 == '\0') {
        return uVar6 & 0xffffff00;
      }
      pbVar7 = (byte *)*puVar3;
      local_c = puVar3[1];
    }
    bVar2 = *pbVar7;
    uVar6 = (uint)bVar2;
    *(undefined4 *)(*param_1 + 0x14) = 0x4e;
    *(uint *)(*param_1 + 0x18) = uVar5;
    *(uint *)(*param_1 + 0x1c) = uVar6;
    (**(code **)(*param_1 + 4))(param_1,1);
    if (0x1f < bVar4) {
      *(undefined4 *)(*param_1 + 0x14) = 0x1a;
      *(uint *)(*param_1 + 0x18) = uVar5;
      (**(code **)*param_1)(param_1);
    }
    if (bVar4 < 0x10) {
      *(byte *)((int)param_1 + uVar5 + 0xca) = bVar2 & 0xf;
      bVar4 = (byte)((int)uVar6 >> 4);
      *(byte *)((int)param_1 + uVar5 + 0xda) = bVar4;
      if (bVar4 < *(byte *)((int)param_1 + uVar5 + 0xca)) {
        *(undefined4 *)(*param_1 + 0x14) = 0x1b;
        *(uint *)(*param_1 + 0x18) = uVar6;
        (**(code **)*param_1)(param_1);
      }
    }
    else {
      *(byte *)((int)param_1 + uVar5 + 0xda) = bVar2;
    }
  } while( true );
}

