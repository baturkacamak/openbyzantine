// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x44d360
// name: FUN_0044d360
// ranges: 0x44d360-0x44d38f
// calls: 0x44d360, 0x453120

undefined4 __cdecl FUN_0044d360(LPCSTR param_1)

{
  BOOL BVar1;
  uint uVar2;
  
  BVar1 = DeleteFileA(param_1);
  uVar2 = 0;
  if (BVar1 == 0) {
    uVar2 = GetLastError();
  }
  if (uVar2 != 0) {
    FUN_00453120(uVar2);
    return 0xffffffff;
  }
  return 0;
}
