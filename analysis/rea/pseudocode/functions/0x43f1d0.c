// rea decompile_all
// name: FUN_0043f1d0
// address: 0x43f1d0
// body: available
// range: 0x43f1d0-0x43f2e8 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


uint __cdecl FUN_0043f1d0(int *param_1)

{
  int *piVar1;
  byte bVar2;
  undefined4 *puVar3;
  uint uVar4;
  int iVar5;
  byte *pbVar6;
  
  puVar3 = (undefined4 *)param_1[5];
  pbVar6 = (byte *)*puVar3;
  iVar5 = puVar3[1];
  while( true ) {
    if (iVar5 == 0) {
      uVar4 = (*(code *)puVar3[3])(param_1);
      if ((char)uVar4 == '\0') {
        return uVar4 & 0xffffff00;
      }
      pbVar6 = (byte *)*puVar3;
      iVar5 = puVar3[1];
    }
    bVar2 = *pbVar6;
    while( true ) {
      pbVar6 = pbVar6 + 1;
      iVar5 = iVar5 + -1;
      if (bVar2 == 0xff) break;
      *(int *)(param_1[100] + 0x58) = *(int *)(param_1[100] + 0x58) + 1;
      *puVar3 = pbVar6;
      puVar3[1] = iVar5;
      if (iVar5 == 0) {
        uVar4 = (*(code *)puVar3[3])(param_1);
        if ((char)uVar4 == '\0') {
          return uVar4 & 0xffffff00;
        }
        pbVar6 = (byte *)*puVar3;
        iVar5 = puVar3[1];
      }
      bVar2 = *pbVar6;
    }
    do {
      if (iVar5 == 0) {
        uVar4 = (*(code *)puVar3[3])(param_1);
        if ((char)uVar4 == '\0') {
          return uVar4 & 0xffffff00;
        }
        pbVar6 = (byte *)*puVar3;
        iVar5 = puVar3[1];
      }
      iVar5 = iVar5 + -1;
      uVar4 = (uint)*pbVar6;
      pbVar6 = pbVar6 + 1;
    } while (uVar4 == 0xff);
    if (uVar4 != 0) break;
    piVar1 = (int *)(param_1[100] + 0x58);
    *piVar1 = *piVar1 + 2;
    *puVar3 = pbVar6;
    puVar3[1] = iVar5;
  }
  if (*(int *)(param_1[100] + 0x58) != 0) {
    *(undefined4 *)(*param_1 + 0x14) = 0x70;
    *(undefined4 *)(*param_1 + 0x18) = *(undefined4 *)(param_1[100] + 0x58);
    *(uint *)(*param_1 + 0x1c) = uVar4;
    (**(code **)(*param_1 + 4))(param_1,0xffffffff);
    *(undefined4 *)(param_1[100] + 0x58) = 0;
  }
  param_1[0x5e] = uVar4;
  *puVar3 = pbVar6;
  puVar3[1] = iVar5;
  return 1;
}

