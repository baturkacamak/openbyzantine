// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x43afe0
// name: FUN_0043afe0
// ranges: 0x43afe0-0x43b06c
// calls: 0x43a80c, 0x43afe0

undefined4 __cdecl FUN_0043afe0(undefined4 *param_1)

{
  undefined4 uVar1;
  LONG LVar2;
  
  SetThreadPriority((HANDLE)param_1[0x24],2);
  uVar1 = FUN_0043a80c(param_1);
  CloseHandle((HANDLE)param_1[0x24]);
  while( true ) {
    LVar2 = InterlockedExchange(param_1 + 0x25,1);
    if (LVar2 == 0) break;
    Sleep(2);
  }
  param_1[0x24] = 0;
  InterlockedExchange(param_1 + 0x25,0);
  return uVar1;
}
