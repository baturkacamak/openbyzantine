// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4177c7
// name: FUN_004177c7
// ranges: 0x4177c7-0x4179ab
// calls: 0x4063e0, 0x4177c7, 0x418040, 0x44c520

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 __cdecl FUN_004177c7(char *param_1)

{
  undefined4 uVar1;
  char *pcVar2;
  int *piVar3;
  DWORD DVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int local_c;
  char *local_8;
  
  if (param_1 == (char *)0x0) {
    DAT_004603d4 = 0xffffffff;
    DAT_0045f23c = DAT_00487508 * 2 + 0x14a;
    DAT_0045f22c = 0;
    DAT_0045f230 = 1;
    DAT_0045f234 = 1;
    uVar1 = 0;
  }
  else if (DAT_0045f210 == 0) {
    DAT_0045f204 = 1;
    uVar1 = 1;
  }
  else {
    if (DAT_0045f234 != 0) {
      DAT_0045f234 = 0;
      _DAT_0045f238 = timeGetTime();
    }
    for (local_c = 0; local_c < DAT_0045f22c; local_c = local_c + 1) {
      pcVar2 = FUN_0044c520(param_1,'\n');
      param_1 = pcVar2 + 1;
    }
    if (*param_1 == '\0') {
      uVar1 = 1;
    }
    else {
      local_8 = param_1;
      for (local_c = 0; local_c < DAT_0045f230; local_c = local_c + 1) {
        local_8 = FUN_0044c520(local_8,'\n');
        local_8 = local_8 + 1;
      }
      if (DAT_0045c060 == 0) {
        DAT_0045f204 = 1;
      }
      local_8[-1] = '\0';
      iVar7 = 3;
      iVar6 = 3;
      iVar5 = 0;
      piVar3 = (int *)FUN_00418040(DAT_0046e82c);
      FUN_004063e0(param_1,5,DAT_0045f23c,1,piVar3,iVar5,iVar6,iVar7);
      local_8[-1] = '\n';
      DVar4 = timeGetTime();
      if (1000 < DVar4 - _DAT_0045f238) {
        if (*local_8 == '\0') {
          DAT_0045f230 = DAT_0045f230 + -1;
        }
        _DAT_0045f238 = DVar4;
        if (DAT_0045f23c < 0x14b) {
          DAT_0045f22c = DAT_0045f22c + 1;
        }
        else {
          DAT_0045f23c = DAT_0045f23c - DAT_00487508;
          DAT_0045f230 = DAT_0045f230 + 1;
        }
      }
      uVar1 = 0;
    }
  }
  return uVar1;
}
