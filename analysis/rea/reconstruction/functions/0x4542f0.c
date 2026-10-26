// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4542f0
// name: ___wtomb_environ
// ranges: 0x4542f0-0x454379
// calls: 0x44c2f0, 0x4574b0

/* Library Function - Single Match
    ___wtomb_environ
   
   Library: Visual Studio 1998 Release */

int __cdecl ___wtomb_environ(void)

{
  SIZE_T cbMultiByte;
  uchar *lpMultiByteStr;
  int iVar1;
  int *piVar2;
  
  iVar1 = *DAT_00462ca4;
  piVar2 = DAT_00462ca4;
  while( true ) {
    if (iVar1 == 0) {
      return 0;
    }
    cbMultiByte = WideCharToMultiByte(1,0,(LPCWSTR)*piVar2,-1,(LPSTR)0x0,0,(LPCSTR)0x0,(LPBOOL)0x0);
    if (cbMultiByte == 0) {
      return -1;
    }
    lpMultiByteStr = (uchar *)FUN_0044c2f0(cbMultiByte);
    if (lpMultiByteStr == (uchar *)0x0) {
      return -1;
    }
    iVar1 = WideCharToMultiByte(1,0,(LPCWSTR)*piVar2,-1,(LPSTR)lpMultiByteStr,cbMultiByte,
                                (LPCSTR)0x0,(LPBOOL)0x0);
    if (iVar1 == 0) break;
    piVar2 = piVar2 + 1;
    FUN_004574b0(lpMultiByteStr,0);
    iVar1 = *piVar2;
  }
  return -1;
}
