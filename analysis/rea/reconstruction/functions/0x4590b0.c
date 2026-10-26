// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4590b0
// name: FUN_004590b0
// ranges: 0x4590b0-0x459151
// calls: 0x4590b0

int __cdecl FUN_004590b0(int param_1,uint param_2)

{
  int iVar1;
  
  if ((param_2._2_2_ & 0x7ff0) != 0x7ff0) {
    if ((param_2 & 0x7fffffff) == 0 && param_1 == 0) {
      return (-(uint)((param_2 & 0x80000000) == 0) & 0x20) + 0x20;
    }
    if (((param_2 & 0x7ff00000) == 0) && (((param_2 & 0xfffff) != 0 || (param_1 != 0)))) {
      return (-(uint)((param_2 & 0x80000000) == 0) & 0x70) + 0x10;
    }
    return (-(uint)((param_2 & 0x80000000) == 0) & 0xf8) + 8;
  }
  iVar1 = __sptype(param_1,param_2);
  if (iVar1 == 1) {
    return 0x200;
  }
  if (iVar1 != 2) {
    if (iVar1 != 3) {
      return 1;
    }
    return 2;
  }
  return 4;
}
