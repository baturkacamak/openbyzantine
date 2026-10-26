// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4162a0
// name: FUN_004162a0
// ranges: 0x4162a0-0x416374
// calls: 0x4162a0, 0x416375

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_004162a0(void)

{
  int iVar1;
  undefined4 uVar2;
  undefined4 local_24 [4];
  ushort local_14;
  ushort local_10;
  ushort local_c;
  
  local_24[0] = 0x20;
  iVar1 = (**(code **)(*DAT_004869d4 + 0x54))(DAT_004869d4,local_24);
  if (iVar1 == 0) {
    DAT_00485c7c = local_14;
    DAT_00485c86 = local_10;
    DAT_00485c84 = local_c;
    _DAT_00485c78 = FUN_00416375(local_14);
    _DAT_00485c80 = FUN_00416375(DAT_00485c86);
    DAT_00485c8c = FUN_00416375(DAT_00485c84);
    if (DAT_00485c86 == 0x7e0) {
      DAT_00485c86 = 0x7c0;
      _DAT_00485c80 = _DAT_00485c80 + 1;
    }
    DAT_00485c88 = (ushort)(0x1f << (DAT_00485c78 & 0x1f)) | 0x1f;
    uVar2 = 0;
  }
  else {
    uVar2 = 1;
  }
  return uVar2;
}
