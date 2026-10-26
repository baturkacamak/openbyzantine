// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x422843
// name: FUN_00422843
// ranges: 0x422843-0x4228b4
// calls: 0x422843

int __cdecl FUN_00422843(undefined4 param_1)

{
  LONG LVar1;
  
  while (DAT_0045fd20 != 0) {
    LVar1 = InterlockedExchange((LONG *)&DAT_004618b0,1);
    if (LVar1 == 0) break;
    Sleep(100);
  }
  if (DAT_0045fd28 != -1) {
    (**(code **)(*DAT_0045fd14 + 0x18))(DAT_0045fd14,param_1);
  }
  InterlockedExchange((LONG *)&DAT_004618b0,0);
  return DAT_0045fd28;
}
