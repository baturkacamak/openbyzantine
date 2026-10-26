// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x44fb20
// name: __close
// ranges: 0x44fb20-0x44fbe6
// calls: 0x453120, 0x455330, 0x4553c0

/* Library Function - Single Match
    __close
   
   Library: Visual Studio 1998 Release */

int __cdecl __close(int _FileHandle)

{
  int *piVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  HANDLE hObject;
  BOOL BVar5;
  DWORD DVar6;
  
  if (DAT_004a7710 <= (uint)_FileHandle) {
    DAT_00462c74 = 9;
    DAT_00462c78 = 0;
    return -1;
  }
  piVar1 = (int *)((int)&DAT_004a7720 + ((int)(_FileHandle & 0xffffffe7U) >> 3));
  iVar2 = (_FileHandle & 0x1fU) * 8;
  if ((*(byte *)(*piVar1 + 4 + iVar2) & 1) == 0) {
    DAT_00462c74 = 9;
    DAT_00462c78 = 0;
    return -1;
  }
  if ((_FileHandle == 1) || (_FileHandle == 2)) {
    iVar3 = FUN_004553c0(1);
    iVar4 = FUN_004553c0(2);
    if (iVar3 != iVar4) goto LAB_0044fb7b;
  }
  else {
LAB_0044fb7b:
    hObject = (HANDLE)FUN_004553c0(_FileHandle);
    BVar5 = CloseHandle(hObject);
    if (BVar5 == 0) {
      DVar6 = GetLastError();
      goto LAB_0044fb9b;
    }
  }
  DVar6 = 0;
LAB_0044fb9b:
  FUN_00455330(_FileHandle);
  if (DVar6 == 0) {
    *(undefined1 *)(*piVar1 + 4 + iVar2) = 0;
    return 0;
  }
  FUN_00453120(DVar6);
  return -1;
}
