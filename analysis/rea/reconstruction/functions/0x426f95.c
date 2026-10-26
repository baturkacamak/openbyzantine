// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x426f95
// name: FUN_00426f95
// ranges: 0x426f95-0x426fc3
// calls: 0x426f95

void __cdecl FUN_00426f95(int param_1)

{
  DWORD DVar1;
  DWORD DVar2;
  
  DVar1 = GetTickCount();
  do {
    DVar2 = GetTickCount();
  } while ((int)DVar2 < (int)(DVar1 + param_1));
  return;
}
