// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x44d2d0
// name: FUN_0044d2d0
// ranges: 0x44d2d0-0x44d33e
// calls: 0x44ac40, 0x44c2f0, 0x44d2d0, 0x4510d0

LPVOID __cdecl FUN_0044d2d0(LPVOID param_1,uint param_2)

{
  LPVOID pvVar1;
  int iVar2;
  
  if (param_1 == (LPVOID)0x0) {
    pvVar1 = (LPVOID)FUN_0044c2f0(param_2);
    return pvVar1;
  }
  if (param_2 != 0) {
    do {
      if (param_2 < 0xffffffe1) {
        pvVar1 = HeapReAlloc(DAT_004a7820,0,param_1,param_2);
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
      iVar2 = FUN_004510d0(param_2);
    } while (iVar2 != 0);
    return (LPVOID)0x0;
  }
  FUN_0044ac40(param_1);
  return (LPVOID)0x0;
}
