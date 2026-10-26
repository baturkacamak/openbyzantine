// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x455d90
// name: FUN_00455d90
// ranges: 0x455d90-0x455fc6
// calls: 0x44ac40, 0x453040, 0x453120, 0x455d90

HANDLE __cdecl FUN_00455d90(int param_1,LPCSTR param_2,char *param_3,LPVOID param_4)

{
  undefined4 *puVar1;
  bool bVar2;
  uint uVar3;
  undefined4 *puVar4;
  BOOL BVar5;
  DWORD DVar6;
  char *pcVar7;
  int iVar8;
  uint uVar9;
  uint *puVar10;
  uint uVar11;
  _STARTUPINFOA *p_Var12;
  uint uVar13;
  HANDLE local_60;
  DWORD local_5c;
  char *local_58;
  _PROCESS_INFORMATION local_54;
  _STARTUPINFOA local_44;
  
  local_5c = 0;
  bVar2 = false;
  switch(param_1) {
  case 0:
  case 1:
  case 2:
  case 3:
    goto switchD_00455daa_caseD_0;
  case 4:
    bVar2 = true;
switchD_00455daa_caseD_0:
    local_58 = param_3;
    break;
  default:
    DAT_00462c74 = 0x16;
    DAT_00462c78 = 0;
    return (HANDLE)0xffffffff;
  }
  while (*param_3 != '\0') {
    do {
      pcVar7 = param_3;
      param_3 = pcVar7 + 1;
    } while (*param_3 != '\0');
    pcVar7 = pcVar7 + 2;
    if (*pcVar7 != '\0') {
      *param_3 = ' ';
      param_3 = pcVar7;
    }
  }
  p_Var12 = &local_44;
  for (iVar8 = 0x11; iVar8 != 0; iVar8 = iVar8 + -1) {
    p_Var12->cb = 0;
    p_Var12 = (_STARTUPINFOA *)&p_Var12->lpReserved;
  }
  local_44.cb = 0x44;
  uVar9 = DAT_004a7710;
  do {
    uVar13 = uVar9;
    if (uVar13 == 0) break;
    uVar9 = uVar13 - 1;
  } while (*(char *)(*(int *)((int)&DAT_004a7720 + ((int)(uVar9 & 0xffffffe7) >> 3)) + 4 +
                    (uVar9 & 0x1f) * 8) == '\0');
  local_44.cbReserved2 = (short)uVar13 * 5 + 4;
  local_44.lpReserved2 = FUN_00453040((uint)local_44.cbReserved2,1);
  uVar9 = 0;
  *(uint *)local_44.lpReserved2 = uVar13;
  puVar10 = (uint *)((int)local_44.lpReserved2 + 4);
  puVar4 = (undefined4 *)((int)local_44.lpReserved2 + uVar13 + 4);
  if (0 < (int)uVar13) {
    do {
      uVar3 = uVar9 & 0xffffffe7;
      uVar11 = uVar9 & 0x1f;
      uVar9 = uVar9 + 1;
      puVar1 = (undefined4 *)(*(int *)((int)&DAT_004a7720 + ((int)uVar3 >> 3)) + uVar11 * 8);
      *(undefined1 *)puVar10 = *(undefined1 *)(puVar1 + 1);
      *puVar4 = *puVar1;
      puVar10 = (uint *)((int)puVar10 + 1);
      puVar4 = puVar4 + 1;
    } while ((int)uVar9 < (int)uVar13);
  }
  if (bVar2) {
    puVar10 = (uint *)((int)local_44.lpReserved2 + 4);
    puVar4 = (undefined4 *)((int)local_44.lpReserved2 + uVar13 + 4);
    if (2 < (int)uVar13) {
      uVar13 = 3;
    }
    if (0 < (int)uVar13) {
      do {
        *(undefined1 *)puVar10 = 0;
        puVar10 = (uint *)((int)puVar10 + 1);
        *puVar4 = 0xffffffff;
        puVar4 = puVar4 + 1;
        uVar13 = uVar13 - 1;
      } while (uVar13 != 0);
    }
    local_5c = 8;
  }
  DAT_00462c74 = 0;
  DAT_00462c78 = 0;
  BVar5 = CreateProcessA(param_2,local_58,(LPSECURITY_ATTRIBUTES)0x0,(LPSECURITY_ATTRIBUTES)0x0,1,
                         local_5c,param_4,(LPCSTR)0x0,&local_44,&local_54);
  DVar6 = GetLastError();
  FUN_0044ac40(local_44.lpReserved2);
  if (BVar5 != 0) {
    if (param_1 == 2) {
      __exit(0);
    }
    else if (param_1 == 0) {
      WaitForSingleObject(local_54.hProcess,0xffffffff);
      GetExitCodeProcess(local_54.hProcess,(LPDWORD)&local_60);
      CloseHandle(local_54.hProcess);
    }
    else if (param_1 == 4) {
      CloseHandle(local_54.hProcess);
      local_60 = (HANDLE)0x0;
    }
    else {
      local_60 = local_54.hProcess;
    }
    CloseHandle(local_54.hThread);
    return local_60;
  }
  FUN_00453120(DVar6);
  return (HANDLE)0xffffffff;
}
