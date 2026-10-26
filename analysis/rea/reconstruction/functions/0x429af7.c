// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x429af7
// name: FUN_00429af7
// ranges: 0x429af7-0x429b72
// calls: 0x429af7

void FUN_00429af7(void)

{
  int local_8;
  
  for (local_8 = 0; local_8 < 0x18; local_8 = local_8 + 1) {
    if ((&DAT_0046ca98)[local_8] != 0) {
      if ((CDataBoundProperty *)(&DAT_0046ca98)[local_8] != (CDataBoundProperty *)0x0) {
        CDataBoundProperty::_scalar_deleting_destructor_
                  ((CDataBoundProperty *)(&DAT_0046ca98)[local_8],1);
      }
      (&DAT_0046ca98)[local_8] = 0;
    }
  }
  return;
}
