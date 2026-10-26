// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x441cf0
// name: FUN_00441cf0
// ranges: 0x441cf0-0x441d71
// calls: 0x441cf0

int __cdecl FUN_00441cf0(int param_1)

{
  int iVar1;
  int iVar2;
  uint3 uVar3;
  
  uVar3 = (uint3)((uint)param_1 >> 8);
  if ((*(char *)(param_1 + 0x48) != '\0') || (*(char *)(param_1 + 0x108) != '\0')) {
    return (uint)uVar3 << 8;
  }
  if ((((*(int *)(param_1 + 0x24) == 3) && (*(int *)(param_1 + 0x20) == 3)) &&
      (*(int *)(param_1 + 0x28) == 2)) && (*(int *)(param_1 + 100) == 3)) {
    iVar1 = *(int *)(param_1 + 0xc4);
    if ((((*(int *)(iVar1 + 8) == 2) && (*(int *)(iVar1 + 0x5c) == 1)) &&
        ((*(int *)(iVar1 + 0xb0) == 1 &&
         ((*(int *)(iVar1 + 0xc) < 3 && (*(int *)(iVar1 + 0x60) == 1)))))) &&
       (*(int *)(iVar1 + 0xb4) == 1)) {
      iVar2 = *(int *)(param_1 + 0x114);
      uVar3 = (uint3)((uint)iVar2 >> 8);
      if (((*(int *)(iVar1 + 0x24) == iVar2) && (*(int *)(iVar1 + 0x78) == iVar2)) &&
         (*(int *)(iVar1 + 0xcc) == iVar2)) {
        return CONCAT31(uVar3,1);
      }
      return (uint)uVar3 << 8;
    }
    return (uint)uVar3 << 8;
  }
  return (uint)uVar3 << 8;
}
