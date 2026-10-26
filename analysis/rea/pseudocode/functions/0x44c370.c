// rea decompile_all
// name: FUN_0044c370
// address: 0x44c370
// body: available
// range: 0x44c370-0x44c51f inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


DWORD __cdecl FUN_0044c370(undefined4 *param_1)

{
  uint uVar1;
  byte bVar2;
  DWORD DVar3;
  DWORD DVar4;
  int *piVar5;
  int iVar6;
  char *pcVar7;
  char *pcVar8;
  DWORD DVar9;
  uint uVar10;
  
  uVar1 = param_1[4];
  if ((int)param_1[1] < 0) {
    param_1[1] = 0;
  }
  DVar3 = FUN_00450fc0(uVar1,0,1);
  if ((int)DVar3 < 0) {
    return 0xffffffff;
  }
  uVar10 = param_1[3];
  if ((uVar10 & 0x108) == 0) {
    return DVar3 - param_1[1];
  }
  pcVar8 = (char *)param_1[2];
  DVar4 = (int)*param_1 - (int)pcVar8;
  DVar9 = DVar4;
  if ((uVar10 & 3) == 0) {
    if ((uVar10 & 0x80) == 0) {
      DAT_00462c74 = 0x16;
      return 0xffffffff;
    }
  }
  else if ((*(byte *)(*(int *)((int)&DAT_004a7720 + ((int)(uVar1 & 0xffffffe7) >> 3)) + 4 +
                     (uVar1 & 0x1f) * 8) & 0x80) != 0) {
    for (; pcVar8 < (char *)*param_1; pcVar8 = pcVar8 + 1) {
      if (*pcVar8 == '\n') {
        DVar9 = DVar9 + 1;
      }
    }
  }
  if (DVar3 != 0) {
    if ((uVar10 & 1) != 0) {
      if (param_1[1] == 0) {
        return DVar3;
      }
      uVar10 = DVar4 + param_1[1];
      piVar5 = (int *)((int)&DAT_004a7720 + ((int)(uVar1 & 0xffffffe7) >> 3));
      iVar6 = (uVar1 & 0x1f) * 8;
      if ((*(byte *)(*piVar5 + 4 + iVar6) & 0x80) != 0) {
        DVar4 = FUN_00450fc0(uVar1,0,2);
        if (DVar4 == DVar3) {
          pcVar7 = (char *)param_1[2];
          pcVar8 = pcVar7 + uVar10;
          for (; pcVar7 < pcVar8; pcVar7 = pcVar7 + 1) {
            if (*pcVar7 == '\n') {
              uVar10 = uVar10 + 1;
            }
          }
          bVar2 = *(byte *)((int)param_1 + 0xd) & 0x20;
        }
        else {
          FUN_00450fc0(uVar1,DVar3,0);
          if (((0x200 < uVar10) || ((param_1[3] & 8) == 0)) ||
             (uVar10 = 0x200, (param_1[3] & 0x400) != 0)) {
            uVar10 = param_1[6];
          }
          bVar2 = *(byte *)(*piVar5 + 4 + iVar6) & 4;
        }
        if (bVar2 != 0) {
          uVar10 = uVar10 + 1;
        }
      }
      DVar3 = DVar3 - uVar10;
    }
    return DVar9 + DVar3;
  }
  return DVar9;
}

