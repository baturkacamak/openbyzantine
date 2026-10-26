// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x450fc0
// name: FUN_00450fc0
// ranges: 0x450fc0-0x45107b
// calls: 0x450fc0, 0x453120, 0x4553c0

DWORD __cdecl FUN_00450fc0(uint param_1,LONG param_2,DWORD param_3)

{
  int *piVar1;
  int iVar2;
  byte *pbVar3;
  HANDLE hFile;
  DWORD DVar4;
  uint uVar5;
  
  if (param_1 < DAT_004a7710) {
    piVar1 = (int *)((int)&DAT_004a7720 + ((int)(param_1 & 0xffffffe7) >> 3));
    iVar2 = (param_1 & 0x1f) * 8;
    if ((*(byte *)(*piVar1 + 4 + iVar2) & 1) != 0) {
      hFile = (HANDLE)FUN_004553c0(param_1);
      if (hFile == (HANDLE)0xffffffff) {
        DAT_00462c74 = 9;
        return 0xffffffff;
      }
      DVar4 = SetFilePointer(hFile,param_2,(PLONG)0x0,param_3);
      uVar5 = 0;
      if (DVar4 == 0xffffffff) {
        uVar5 = GetLastError();
      }
      if (uVar5 != 0) {
        FUN_00453120(uVar5);
        return 0xffffffff;
      }
      pbVar3 = (byte *)(*piVar1 + 4 + iVar2);
      *pbVar3 = *pbVar3 & 0xfd;
      return DVar4;
    }
  }
  DAT_00462c74 = 9;
  DAT_00462c78 = 0;
  return 0xffffffff;
}
