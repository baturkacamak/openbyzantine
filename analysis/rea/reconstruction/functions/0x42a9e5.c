// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x42a9e5
// name: FUN_0042a9e5
// ranges: 0x42a9e5-0x42aa19
// calls: 0x42a9e5

HANDLE __cdecl FUN_0042a9e5(LPCSTR param_1)

{
  HANDLE pvVar1;
  
  pvVar1 = CreateFileA(param_1,0x80000000,1,(LPSECURITY_ATTRIBUTES)0x0,3,0x80,(HANDLE)0x0);
  return pvVar1;
}
