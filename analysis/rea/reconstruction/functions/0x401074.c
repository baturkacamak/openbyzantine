// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x401074
// name: FUN_00401074
// ranges: 0x401074-0x401149
// calls: 0x401074, 0x4161f6, 0x4240ae

void FUN_00401074(void)

{
  HWND hWnd;
  char *lpText;
  LPCSTR lpCaption;
  UINT uType;
  int local_64;
  undefined4 local_60;
  char local_5c [80];
  int local_c;
  int local_8;
  
  local_c = (int)DAT_00486a0a;
  local_8 = DAT_00486a0c + -0x14;
  if ((((DAT_0045c060 != 0) && (DAT_00464880 != (void *)0x0)) && (-1 < DAT_00486a0a)) &&
     (((DAT_00486a0a < 0x281 && (0x13 < DAT_00486a0c)) && (DAT_00486a0c < 0x1a5)))) {
    FUN_004240ae(DAT_00464880,&local_c,&local_64);
    _sprintf(local_5c,s_X____d_Y____d_0045c094,local_64,local_60);
    uType = 0;
    lpCaption = (LPCSTR)0x0;
    lpText = local_5c;
    hWnd = (HWND)FUN_004161f6();
    MessageBoxA(hWnd,lpText,lpCaption,uType);
  }
  return;
}
