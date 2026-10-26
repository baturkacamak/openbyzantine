// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x406041
// name: FUN_00406041
// ranges: 0x406041-0x406129
// calls: 0x405c19, 0x406041, 0x44ac40, 0x44c2f0

bool __cdecl FUN_00406041(LPCSTR param_1,LPVOID param_2)

{
  HANDLE hFile;
  ushort *lpBuffer;
  undefined4 uVar1;
  SIZE_T nNumberOfBytesToRead;
  DWORD local_8;
  
  hFile = CreateFileA(param_1,0x80000000,0,(LPSECURITY_ATTRIBUTES)0x0,3,0x80,(HANDLE)0x0);
  if (hFile != (HANDLE)0xffffffff) {
    ReadFile(hFile,param_2,0x12,&local_8,(LPOVERLAPPED)0x0);
    nNumberOfBytesToRead =
         ((int)(uint)*(byte *)((int)param_2 + 0x10) >> 3) * (uint)*(ushort *)((int)param_2 + 0xc) *
         (uint)*(ushort *)((int)param_2 + 0xe);
    lpBuffer = (ushort *)FUN_0044c2f0(nNumberOfBytesToRead);
    uVar1 = FUN_0044c2f0(nNumberOfBytesToRead);
    *(undefined4 *)((int)param_2 + 0x14) = uVar1;
    ReadFile(hFile,lpBuffer,nNumberOfBytesToRead,&local_8,(LPOVERLAPPED)0x0);
    CloseHandle(hFile);
    FUN_00405c19((int)param_2,lpBuffer);
    FUN_0044ac40(lpBuffer);
  }
  return hFile != (HANDLE)0xffffffff;
}
