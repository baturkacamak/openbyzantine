// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x424dc0
// name: FUN_00424dc0
// ranges: 0x424dc0-0x42515e
// calls: 0x401000, 0x408a1e, 0x408d3f, 0x40ac38, 0x40ac43, 0x41d6df, 0x420888, 0x42118c, 0x424dc0, 0x425218, 0x425a54, 0x4269ec, 0x427135, 0x44a460, 0x44bc20

int __cdecl FUN_00424dc0(char *param_1)

{
  bool bVar1;
  undefined3 extraout_var;
  FILE *_File;
  int iVar2;
  int local_10;
  int local_8;
  
  local_8 = 0;
  bVar1 = FUN_00425a54(150000);
  if (CONCAT31(extraout_var,bVar1) == 0) {
    FUN_0041d6df(7);
    local_8 = 1;
  }
  else {
    _File = (FILE *)FUN_0044bc20(param_1,&DAT_0045fe10);
    if (_File == (FILE *)0x0) {
      FUN_0040ac43();
      FUN_0040ac38();
      local_8 = 1;
    }
    else {
      if (DAT_0045da78 == 1) {
        DAT_0045da78 = 2;
      }
      FUN_0044a460((char *)&DAT_0045da78,4,1,_File);
      _fputs(&DAT_004859c0,_File);
      _fputc(0,_File);
      _fputs(&DAT_00484e30,_File);
      _fputc(0,_File);
      FUN_00420888(DAT_0046e82c,_File);
      FUN_0044a460((char *)&DAT_00482428,2,1,_File);
      for (local_10 = 0; local_10 < 0x15e; local_10 = local_10 + 1) {
        iVar2 = FUN_00425218(local_10,_File);
        local_8 = local_8 + iVar2;
      }
      FUN_0044a460(&DAT_0047e2f0,2,3000,_File);
      FUN_00408a1e(_File);
      FUN_0044a460((char *)&DAT_0045f210,4,1,_File);
      FUN_0044a460((char *)&DAT_00460668,4,1,_File);
      FUN_0044a460((char *)&DAT_0046066c,4,1,_File);
      if (DAT_0045f214 != 0) {
        for (local_10 = 0; *(char *)(local_10 + DAT_0045f214) != '\0'; local_10 = local_10 + 1) {
          _fputc((int)*(char *)(local_10 + DAT_0045f214),_File);
        }
      }
      _fputc(0,_File);
      FUN_0044a460((char *)&DAT_0045c060,4,1,_File);
      if (DAT_0045c060 == 0) {
        FUN_0044a460((char *)&DAT_0048253e,2,1,_File);
      }
      else {
        FUN_00401000(_File);
      }
      iVar2 = FUN_004269ec(_File);
      local_8 = local_8 + iVar2;
      for (local_10 = 0; local_10 < 0x15e; local_10 = local_10 + 1) {
        if (*(int *)(&DAT_00464300 + local_10 * 4) == 0) {
          _fputc(0xff,_File);
        }
        else {
          _fputc(0,_File);
          FUN_0044a460(*(char **)(&DAT_00464300 + local_10 * 4),0x18,1,_File);
        }
      }
      FUN_00427135(_File);
      for (local_10 = 0; local_10 < 0x15e; local_10 = local_10 + 1) {
        if (*(int *)(&DAT_00464f60 + local_10 * 4) == 0) {
          _fputc(0xff,_File);
        }
        else {
          _fputc(0,_File);
          FUN_0044a460(*(char **)(&DAT_00464f60 + local_10 * 4),0x14,1,_File);
        }
      }
      FUN_0044a460((char *)&DAT_00464878,4,1,_File);
      FUN_00408d3f(_File);
      FUN_0042118c(_File);
      _fclose(_File);
    }
  }
  return local_8;
}
