// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x40ea8b
// name: FUN_0040ea8b
// ranges: 0x40ea8b-0x40ebe0
// calls: 0x40e23e, 0x40e25f, 0x40e29f, 0x40ea8b

void FUN_0040ea8b(void)

{
  int local_c;
  
  if (((DAT_0045e538 == 0) && (DAT_0045dbd0 != 0)) && (DAT_00476338 < 0x20)) {
    DAT_0045e538 = 1;
    *(int *)(PTR_PTR_0045e534 + (uint)DAT_00476338 * 0x24 + 0xc) =
         *(int *)(PTR_PTR_0045e534 + (uint)DAT_00476338 * 0x24 + 0xc) + 1;
    local_c = *(int *)(PTR_PTR_0045e534 + (uint)DAT_00476338 * 0x24 + 0xc);
    if (*(int *)(PTR_PTR_0045e534 + (uint)DAT_00476338 * 0x24 + 0x10) == local_c) {
      *(undefined4 *)(PTR_PTR_0045e534 + (uint)DAT_00476338 * 0x24 + 0xc) = 0;
      local_c = *(int *)(PTR_PTR_0045e534 + (uint)DAT_00476338 * 0x24 + 0xc);
    }
    DAT_0045dbd0 = 0;
    FUN_0040e25f();
    (**(code **)(*DAT_0045dbd4 + 0x1c))
              (DAT_0045dbd4,0,0,
               *(undefined4 *)(PTR_PTR_0045e534 + (uint)DAT_00476338 * 0x24 + local_c * 4 + 0x14),
               &DAT_0046c450,0x10);
    FUN_0040e29f();
    FUN_0040e23e();
    DAT_0045dbd0 = 1;
    DAT_0045e538 = 0;
  }
  return;
}
