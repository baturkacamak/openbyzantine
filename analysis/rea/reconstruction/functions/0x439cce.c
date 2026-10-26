// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x439cce
// name: FUN_00439cce
// ranges: 0x439cce-0x439d70
// calls: 0x439cce

undefined4 __fastcall FUN_00439cce(int param_1)

{
  int iVar1;
  LONG LVar2;
  
  InterlockedExchange((LONG *)(param_1 + 0x20),1);
  while (LVar2 = InterlockedExchange((LONG *)(param_1 + 0x94),1), LVar2 != 0) {
    Sleep(2);
  }
  iVar1 = *(int *)(param_1 + 0x90);
  InterlockedExchange((LONG *)(param_1 + 0x94),0);
  if (iVar1 != 0) {
    WaitForSingleObject(*(HANDLE *)(param_1 + 0x90),5000);
  }
  return 0;
}
