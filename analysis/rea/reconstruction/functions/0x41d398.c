// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x41d398
// name: FUN_0041d398
// ranges: 0x41d398-0x41d3d4
// calls: 0x416235, 0x41624a, 0x41d398

undefined1 FUN_0041d398(void)

{
  undefined1 uVar1;
  int iVar2;
  
  FUN_0041624a();
  iVar2 = FUN_00416235();
  if (*(int *)(iVar2 + 4) == 0x102) {
    uVar1 = *(undefined1 *)(iVar2 + 8);
  }
  else {
    uVar1 = 0;
  }
  return uVar1;
}
