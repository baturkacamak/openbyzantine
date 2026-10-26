// rea decompile_all
// name: FUN_0044e600
// address: 0x44e600
// body: available
// range: 0x44e600-0x44e862 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


int __cdecl FUN_0044e600(uint param_1,char *param_2,DWORD param_3)

{
  int *piVar1;
  int iVar2;
  byte *pbVar3;
  char cVar4;
  int iVar5;
  BOOL BVar6;
  DWORD DVar7;
  byte bVar8;
  char *pcVar9;
  char *pcVar10;
  char *pcVar11;
  char local_d;
  DWORD local_c;
  DWORD local_8;
  char *local_4;
  
  if (param_1 < DAT_004a7710) {
    piVar1 = (int *)((int)&DAT_004a7720 + ((int)(param_1 & 0xffffffe7) >> 3));
    iVar2 = (param_1 & 0x1f) * 8;
    iVar5 = *piVar1 + iVar2;
    if ((*(byte *)(iVar5 + 4) & 1) != 0) {
      local_c = 0;
      if ((param_3 == 0) || ((*(byte *)(iVar5 + 4) & 2) != 0)) {
        return 0;
      }
      pcVar9 = param_2;
      if (((*(byte *)(iVar5 + 4) & 0x48) != 0) && (*(char *)(iVar5 + 5) != '\n')) {
        *param_2 = *(char *)(iVar5 + 5);
        pcVar9 = param_2 + 1;
        param_3 = param_3 - 1;
        local_c = 1;
        *(undefined1 *)(*piVar1 + 5 + iVar2) = 10;
      }
      BVar6 = ReadFile(*(HANDLE *)(*piVar1 + iVar2),pcVar9,param_3,&local_8,(LPOVERLAPPED)0x0);
      if (BVar6 == 0) {
        DVar7 = GetLastError();
        if (DVar7 == 5) {
          DAT_00462c78 = DVar7;
          DAT_00462c74 = 9;
          return -1;
        }
        if (DVar7 != 0x6d) {
          FUN_00453120(DVar7);
          return -1;
        }
        return 0;
      }
      local_c = local_c + local_8;
      pbVar3 = (byte *)(*piVar1 + 4 + iVar2);
      bVar8 = *pbVar3;
      if ((bVar8 & 0x80) != 0) {
        if ((local_8 == 0) || (*param_2 != '\n')) {
          bVar8 = bVar8 & 0xfb;
        }
        else {
          bVar8 = bVar8 | 4;
        }
        *pbVar3 = bVar8;
        local_4 = param_2 + local_c;
        pcVar9 = param_2;
        pcVar11 = param_2;
        if (param_2 < local_4) {
          do {
            cVar4 = *pcVar9;
            if (cVar4 == '\x1a') {
              pbVar3 = (byte *)(*piVar1 + 4 + iVar2);
              bVar8 = *pbVar3;
              if ((bVar8 & 0x40) == 0) {
                *pbVar3 = bVar8 | 2;
              }
              break;
            }
            if (cVar4 == '\r') {
              if (pcVar9 < local_4 + -1) {
                pcVar10 = pcVar9 + 1;
                if (*pcVar10 == '\n') {
                  pcVar10 = pcVar9 + 2;
                  *pcVar11 = '\n';
                }
                else {
                  *pcVar11 = '\r';
                }
                goto LAB_0044e804;
              }
              pcVar10 = pcVar9 + 1;
              local_c = 0;
              BVar6 = ReadFile(*(HANDLE *)(*piVar1 + iVar2),&local_d,1,&local_8,(LPOVERLAPPED)0x0);
              if (BVar6 == 0) {
                local_c = GetLastError();
              }
              if ((local_c != 0) || (local_8 == 0)) {
LAB_0044e801:
                *pcVar11 = '\r';
                goto LAB_0044e804;
              }
              if ((*(byte *)(*piVar1 + 4 + iVar2) & 0x48) == 0) {
                if ((pcVar11 == param_2) && (local_d == '\n')) {
                  *pcVar11 = '\n';
                  goto LAB_0044e804;
                }
                FUN_00450fc0(param_1,-1,1);
                if (local_d != '\n') goto LAB_0044e801;
              }
              else {
                if (local_d == '\n') {
                  *pcVar11 = '\n';
                  goto LAB_0044e804;
                }
                *pcVar11 = '\r';
                pcVar11 = pcVar11 + 1;
                *(char *)(*piVar1 + 5 + iVar2) = local_d;
              }
            }
            else {
              pcVar10 = pcVar9 + 1;
              *pcVar11 = cVar4;
LAB_0044e804:
              pcVar11 = pcVar11 + 1;
            }
            pcVar9 = pcVar10;
          } while (pcVar10 < local_4);
        }
        local_c = (int)pcVar11 - (int)param_2;
      }
      return local_c;
    }
  }
  DAT_00462c74 = 9;
  DAT_00462c78 = 0;
  return -1;
}

