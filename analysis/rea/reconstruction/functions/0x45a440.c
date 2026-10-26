// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x45a440
// name: FUN_0045a440
// ranges: 0x45a440-0x45a481
// calls: 0x45a440

uint __cdecl FUN_0045a440(byte param_1)

{
  BOOL BVar1;
  byte local_5;
  DWORD local_4;
  
  local_5 = param_1;
  if (DAT_004642e8 != (HANDLE)0xffffffff) {
    BVar1 = WriteConsoleA(DAT_004642e8,&local_5,1,&local_4,(LPVOID)0x0);
    if (BVar1 != 0) {
      return (uint)local_5;
    }
  }
  return 0xffffffff;
}
