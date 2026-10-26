// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x401d29
// name: FUN_00401d29
// ranges: 0x401d29-0x401dde
// calls: 0x401d29, 0x40f135, 0x42382b

undefined4 __cdecl FUN_00401d29(char *param_1)

{
  undefined4 uVar1;
  DWORD DVar2;
  int local_14;
  DWORD local_10;
  uint local_c;
  ushort *local_8;
  
  local_8 = (ushort *)0x0;
  local_14 = DAT_0046487c;
  local_c = DAT_004654d8;
  if (DAT_00464880 == 0) {
    uVar1 = 1;
  }
  else {
    local_10 = timeGetTime();
    local_8 = FUN_0040f135(param_1,&local_14,&local_c,DAT_0045c040,DAT_00468ca0);
    DVar2 = timeGetTime();
    local_10 = DVar2 - local_10;
    if (local_8 == (ushort *)0x0) {
      uVar1 = 1;
    }
    else {
      FUN_0042382b(DAT_00464880);
      uVar1 = 0;
    }
  }
  return uVar1;
}
