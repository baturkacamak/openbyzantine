// rea decompile_all
// name: FUN_00447010
// address: 0x447010
// body: available
// range: 0x447010-0x447115 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_00447010(int param_1,int param_2,int param_3,int param_4)

{
  int iVar1;
  int iVar2;
  int iVar3;
  short *psVar4;
  int *piVar5;
  int iVar6;
  int iVar7;
  byte *pbVar8;
  int local_18c;
  byte local_180 [128];
  undefined1 local_100 [256];
  byte *pbVar9;
  
  iVar6 = *(int *)(*(int *)(param_1 + 0x1a4) + 0x18);
  iVar7 = (param_2 >> 2) * 0x20 + 4;
  iVar1 = (param_3 >> 3) * 0x20 + 2;
  iVar2 = (param_4 >> 2) * 0x20 + 4;
  iVar3 = FUN_00447120(param_1,iVar7,iVar1,iVar2,(int)local_100);
  FUN_004472f0(param_1,iVar7,iVar1,iVar2,iVar3,(int)local_100,local_180);
  pbVar8 = local_180;
  piVar5 = (int *)(iVar6 + (param_2 >> 2) * 0x10);
  local_18c = 4;
  do {
    iVar1 = 8;
    iVar6 = ((param_3 >> 3) * 0x100 + (param_4 >> 2) * 4) * 2;
    do {
      iVar2 = 4;
      psVar4 = (short *)(*piVar5 + iVar6);
      pbVar9 = pbVar8;
      do {
        pbVar8 = pbVar9 + 1;
        iVar2 = iVar2 + -1;
        *psVar4 = *pbVar9 + 1;
        psVar4 = psVar4 + 1;
        pbVar9 = pbVar8;
      } while (iVar2 != 0);
      iVar6 = iVar6 + 0x40;
      iVar1 = iVar1 + -1;
    } while (iVar1 != 0);
    piVar5 = piVar5 + 1;
    local_18c = local_18c + -1;
  } while (local_18c != 0);
  return;
}

