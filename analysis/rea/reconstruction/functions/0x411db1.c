// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x411db1
// name: FUN_00411db1
// ranges: 0x411db1-0x411dfb
// calls: 0x411db1, 0x44c370

uint __cdecl FUN_00411db1(undefined4 *param_1,int param_2)

{
  DWORD DVar1;
  
  if ((param_2 != -1) && (DVar1 = FUN_0044c370(param_1), param_2 <= (int)DVar1)) {
    return 1;
  }
  return param_1[3] & 0x10;
}
