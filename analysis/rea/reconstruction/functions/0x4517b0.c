// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4517b0
// name: __access
// ranges: 0x4517b0-0x4517fc
// calls: 0x453120

/* Library Function - Single Match
    __access
   
   Library: Visual Studio 1998 Release */

int __cdecl __access(char *_Filename,int _AccessMode)

{
  DWORD DVar1;
  
  DVar1 = GetFileAttributesA(_Filename);
  if (DVar1 == 0xffffffff) {
    DVar1 = GetLastError();
    FUN_00453120(DVar1);
    return -1;
  }
  if (((DVar1 & 1) != 0) && ((_AccessMode & 2U) != 0)) {
    DAT_00462c74 = 0xd;
    DAT_00462c78 = 5;
    return -1;
  }
  return 0;
}
