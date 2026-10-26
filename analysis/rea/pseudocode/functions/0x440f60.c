// rea decompile_all
// name: FUN_00440f60
// address: 0x440f60
// body: available
// range: 0x440f60-0x44107e inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


int __cdecl FUN_00440f60(int *param_1,int param_2,uint param_3)

{
  int iVar1;
  int *piVar2;
  int iVar3;
  undefined4 *puVar4;
  int iVar5;
  undefined4 *puVar6;
  undefined4 *puVar7;
  uint uVar8;
  undefined4 *local_c;
  
  iVar5 = param_1[1];
  if (0x3b9ac9f0 < param_3) {
    FUN_00441080(param_1,1);
  }
  if ((param_3 & 7) != 0) {
    param_3 = (param_3 - (param_3 & 7)) + 8;
  }
  if ((param_2 < 0) || (1 < param_2)) {
    *(undefined4 *)(*param_1 + 0x14) = 0xc;
    *(int *)(*param_1 + 0x18) = param_2;
    (**(code **)*param_1)(param_1);
  }
  iVar3 = param_2 * 4;
  local_c = (undefined4 *)0x0;
  puVar4 = (undefined4 *)(iVar3 + 0x30 + iVar5);
  puVar7 = (undefined4 *)*puVar4;
  if (puVar7 != (undefined4 *)0x0) {
    do {
      puVar6 = puVar7;
      puVar7 = puVar6;
      if (param_3 <= (uint)puVar6[2]) break;
      puVar7 = (undefined4 *)*puVar6;
      local_c = puVar6;
    } while (puVar7 != (undefined4 *)0x0);
    if (puVar7 != (undefined4 *)0x0) goto LAB_00441066;
  }
  iVar1 = param_3 + 0x10;
  if (local_c == (undefined4 *)0x0) {
    uVar8 = *(uint *)(&DAT_0045b3a8 + iVar3);
  }
  else {
    uVar8 = *(uint *)(&DAT_0045b3b0 + iVar3);
  }
  if (1000000000U - iVar1 < uVar8) {
    uVar8 = 1000000000U - iVar1;
  }
  while (puVar7 = (undefined4 *)FUN_004422e0(param_1,uVar8 + iVar1), puVar7 == (undefined4 *)0x0) {
    uVar8 = uVar8 >> 1;
    if (uVar8 < 0x32) {
      FUN_00441080(param_1,2);
    }
  }
  piVar2 = (int *)(iVar5 + 0x48);
  *piVar2 = *piVar2 + uVar8 + iVar1;
  *puVar7 = 0;
  puVar7[1] = 0;
  puVar7[2] = uVar8 + param_3;
  if (local_c == (undefined4 *)0x0) {
    local_c = puVar4;
  }
  *local_c = puVar7;
LAB_00441066:
  iVar5 = puVar7[1];
  puVar7[1] = iVar5 + param_3;
  puVar7[2] = puVar7[2] - param_3;
  return iVar5 + 0x10 + (int)puVar7;
}

