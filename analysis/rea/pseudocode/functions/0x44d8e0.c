// rea decompile_all
// name: FUN_0044d8e0
// address: 0x44d8e0
// body: available
// range: 0x44d8e0-0x44db0a inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


int __cdecl FUN_0044d8e0(uint param_1,char *param_2,uint param_3)

{
  byte bVar1;
  char cVar2;
  BOOL BVar3;
  char *pcVar4;
  int iVar5;
  char *pcVar6;
  uint local_418;
  DWORD local_414;
  int *local_410;
  int local_40c;
  DWORD local_408;
  char local_404 [1028];
  
  if (param_1 < DAT_004a7710) {
    local_410 = (int *)((int)&DAT_004a7720 + ((int)(param_1 & 0xffffffe7) >> 3));
    local_40c = (param_1 & 0x1f) * 8;
    bVar1 = *(byte *)(*local_410 + 4 + local_40c);
    if ((bVar1 & 1) != 0) {
      iVar5 = 0;
      local_408 = 0;
      if (param_3 == 0) {
        return 0;
      }
      if ((bVar1 & 0x20) != 0) {
        FUN_00450fc0(param_1,0,2);
      }
      if ((*(byte *)((undefined4 *)(local_40c + *local_410) + 1) & 0x80) == 0) {
        BVar3 = WriteFile(*(HANDLE *)(local_40c + *local_410),param_2,param_3,&local_414,
                          (LPOVERLAPPED)0x0);
        if (BVar3 == 0) {
LAB_0044da25:
          local_418 = GetLastError();
        }
        else {
          local_418 = 0;
          local_408 = local_414;
        }
      }
      else {
        local_418 = 0;
        pcVar6 = param_2;
        do {
          if (param_3 <= (uint)((int)pcVar6 - (int)param_2)) break;
          pcVar4 = local_404;
          do {
            if (param_3 <= (uint)((int)pcVar6 - (int)param_2)) break;
            cVar2 = *pcVar6;
            pcVar6 = pcVar6 + 1;
            if (cVar2 == '\n') {
              *pcVar4 = '\r';
              iVar5 = iVar5 + 1;
              pcVar4 = pcVar4 + 1;
            }
            *pcVar4 = cVar2;
            pcVar4 = pcVar4 + 1;
          } while ((int)pcVar4 - (int)local_404 < 0x400);
          BVar3 = WriteFile(*(HANDLE *)(*local_410 + local_40c),local_404,
                            (int)pcVar4 - (int)local_404,&local_414,(LPOVERLAPPED)0x0);
          if (BVar3 == 0) goto LAB_0044da25;
          local_408 = local_408 + local_414;
        } while ((int)pcVar4 - (int)local_404 <= (int)local_414);
      }
      if (local_408 != 0) {
        return local_408 - iVar5;
      }
      if (local_418 == 0) {
        if (((*(byte *)(*local_410 + 4 + local_40c) & 0x40) != 0) && (*param_2 == '\x1a')) {
          return 0;
        }
        DAT_00462c74 = 0x1c;
        DAT_00462c78 = 0;
        return -1;
      }
      if (local_418 != 5) {
        FUN_00453120(local_418);
        return -1;
      }
      DAT_00462c74 = 9;
      DAT_00462c78 = local_418;
      return -1;
    }
  }
  DAT_00462c74 = 9;
  DAT_00462c78 = 0;
  return -1;
}

