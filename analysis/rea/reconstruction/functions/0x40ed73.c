// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x40ed73
// name: FUN_0040ed73
// ranges: 0x40ed73-0x40edd1
// calls: 0x40ed73

void FUN_0040ed73(void)

{
  LONG LVar1;
  
  if (DAT_0045e540 != (HANDLE)0x0) {
    while( true ) {
      LVar1 = InterlockedExchange((LONG *)&DAT_0045dbe0,1);
      if (LVar1 == 0) break;
      Sleep(10);
    }
    TerminateThread(DAT_0045e540,0);
    DAT_0045e540 = (HANDLE)0x0;
    InterlockedExchange((LONG *)&DAT_0045dbe0,0);
  }
  return;
}
