// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x44d4d0
// name: FUN_0044d4d0
// ranges: 0x44d4d0-0x44d529
// calls: 0x44d4d0, 0x451cb0

void __cdecl FUN_0044d4d0(int *param_1)

{
  int iVar1;
  _SYSTEMTIME local_10;
  
  GetLocalTime(&local_10);
  iVar1 = FUN_00451cb0(local_10._0_4_ & 0xffff,(uint)local_10._0_4_ >> 0x10,(uint)local_10.wDay,
                       local_10._8_4_ & 0xffff,(uint)local_10._8_4_ >> 0x10,local_10._12_4_ & 0xffff
                      );
  if (param_1 != (int *)0x0) {
    *param_1 = iVar1;
  }
  return;
}
