// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x453040
// name: FUN_00453040
// ranges: 0x453040-0x453091
// calls: 0x4510d0, 0x453040

LPVOID __cdecl FUN_00453040(int param_1,int param_2)

{
  LPVOID pvVar1;
  int iVar2;
  uint dwBytes;
  
  dwBytes = param_1 * param_2;
  if (dwBytes == 0) {
    dwBytes = 1;
  }
  do {
    if (dwBytes < 0xffffffe1) {
      pvVar1 = HeapAlloc(DAT_004a7820,8,dwBytes);
    }
    else {
      pvVar1 = (LPVOID)0x0;
    }
    if (pvVar1 != (LPVOID)0x0) {
      return pvVar1;
    }
    if (DAT_004634c0 == 0) {
      return (LPVOID)0x0;
    }
    iVar2 = FUN_004510d0(dwBytes);
  } while (iVar2 != 0);
  return (LPVOID)0x0;
}
