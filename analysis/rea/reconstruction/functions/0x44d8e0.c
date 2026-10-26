// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x44d8e0
// name: FUN_0044d8e0
// ranges: 0x44d8e0-0x44db0a
// calls: 0x44d8e0, 0x450fc0, 0x453120

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
