// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x40fd7b
// name: FUN_0040fd7b
// ranges: 0x40fd7b-0x40fe6f
// calls: 0x40fd7b, 0x40fe70, 0x4102ae

undefined2 FUN_0040fd7b(void)

{
  DWORD DVar1;
  uint local_14;
  undefined2 local_8;
  
  local_8 = 0;
  FUN_004102ae();
  if (DAT_00460660 == 0) {
    if (DAT_004603d0 == 0) {
      DAT_0045c05c = 0;
    }
    for (local_14 = 0; (int)local_14 < 0x15e; local_14 = local_14 + 1) {
      if ((((*(int *)(&DAT_0047dd70 + local_14 * 4) != 0) &&
           (*(char *)(*(int *)(&DAT_0047dd70 + local_14 * 4) + 9) == '\0')) &&
          ((*(char *)(*(int *)(&DAT_0047dd70 + local_14 * 4) + 8) == '\x02' ||
           (*(char *)(*(int *)(&DAT_0047dd70 + local_14 * 4) + 8) == '\x03')))) &&
         (DVar1 = FUN_0040fe70(local_14,(int *)(&DAT_004649e8 + local_14 * 4)), DVar1 == 0)) {
        local_8 = 1;
      }
    }
  }
  else {
    local_8 = 0;
  }
  return local_8;
}
