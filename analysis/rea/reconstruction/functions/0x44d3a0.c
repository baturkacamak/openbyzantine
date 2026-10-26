// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x44d3a0
// name: FUN_0044d3a0
// ranges: 0x44d3a0-0x44d3d4
// calls: 0x44d3a0, 0x453120

undefined4 __cdecl FUN_0044d3a0(LPCSTR param_1,LPCSTR param_2)

{
  BOOL BVar1;
  uint uVar2;
  
  BVar1 = MoveFileA(param_1,param_2);
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
