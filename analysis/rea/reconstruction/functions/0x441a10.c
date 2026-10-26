// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x441a10
// name: FUN_00441a10
// ranges: 0x441a10-0x441a3c
// calls: 0x441a10

void __cdecl FUN_00441a10(int param_1)

{
  int iVar1;
  
  iVar1 = 1;
  do {
    (**(code **)(*(int *)(param_1 + 4) + 0x24))(param_1,iVar1);
    iVar1 = iVar1 + -1;
  } while (iVar1 != 0);
  *(uint *)(param_1 + 0x10) = (-(uint)(*(char *)(param_1 + 0xc) == '\0') & 0xffffff9c) + 200;
  return;
}
