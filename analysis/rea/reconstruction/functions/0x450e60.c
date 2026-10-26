// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x450e60
// name: FUN_00450e60
// ranges: 0x450e60-0x450ece
// calls: 0x450e60, 0x4553c0

undefined4 __cdecl FUN_00450e60(uint param_1)

{
  HANDLE hFile;
  BOOL BVar1;
  DWORD DVar2;
  undefined4 uVar3;
  
  if ((param_1 < DAT_004a7710) &&
     ((*(byte *)(*(int *)((int)&DAT_004a7720 + ((int)(param_1 & 0xffffffe7) >> 3)) + 4 +
                (param_1 & 0x1f) * 8) & 1) != 0)) {
    hFile = (HANDLE)FUN_004553c0(param_1);
    BVar1 = FlushFileBuffers(hFile);
    DVar2 = 0;
    if (BVar1 == 0) {
      DVar2 = GetLastError();
    }
    uVar3 = 0;
    if (DVar2 != 0) {
      DAT_00462c74 = 9;
      DAT_00462c78 = DVar2;
      return 0xffffffff;
    }
  }
  else {
    DAT_00462c74 = 9;
    uVar3 = 0xffffffff;
  }
  return uVar3;
}
