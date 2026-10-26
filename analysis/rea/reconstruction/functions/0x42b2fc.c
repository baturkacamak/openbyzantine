// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x42b2fc
// name: FUN_0042b2fc
// ranges: 0x42b2fc-0x42b376
// calls: 0x4164a0, 0x42b2fc

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __cdecl FUN_0042b2fc(ushort *param_1,int param_2)

{
  ushort uVar1;
  int local_c;
  ushort *local_8;
  
  if (((DAT_00485c8c != 0) || (_DAT_00485c80 != 5)) || (_DAT_00485c78 != 10)) {
    local_8 = param_1;
    for (local_c = 0; local_c < param_2; local_c = local_c + 1) {
      uVar1 = FUN_004164a0(*local_8);
      *local_8 = uVar1;
      local_8 = local_8 + 1;
    }
  }
  return;
}
