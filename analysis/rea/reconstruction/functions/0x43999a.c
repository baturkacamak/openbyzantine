// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x43999a
// name: FUN_0043999a
// ranges: 0x43999a-0x439a27
// calls: 0x43999a, 0x43b070, 0x43b0f0

void __fastcall FUN_0043999a(int param_1)

{
  if (*(void **)(param_1 + 4) != (void *)0x0) {
    FUN_0043b070(*(void **)(param_1 + 4),1);
  }
  if (*(CDataBoundProperty **)(param_1 + 8) != (CDataBoundProperty *)0x0) {
    CDataBoundProperty::_scalar_deleting_destructor_(*(CDataBoundProperty **)(param_1 + 8),1);
  }
  if (*(void **)(param_1 + 0xc) != (void *)0x0) {
    FUN_0043b0f0(*(void **)(param_1 + 0xc),1);
  }
  return;
}
