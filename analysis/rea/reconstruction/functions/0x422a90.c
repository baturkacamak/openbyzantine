// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x422a90
// name: FUN_00422a90
// ranges: 0x422a90-0x422b03
// calls: 0x422a90

int FUN_00422a90(void)

{
  LONG LVar1;
  
  while (DAT_0045fd20 != 0) {
    LVar1 = InterlockedExchange((LONG *)&DAT_004618b0,1);
    if (LVar1 == 0) break;
    Sleep(100);
  }
  if (DAT_0045fd28 != -1) {
    (**(code **)(*DAT_0045fd14 + 0x30))(DAT_0045fd14,0,0,1);
  }
  InterlockedExchange((LONG *)&DAT_004618b0,0);
  return DAT_0045fd28;
}
