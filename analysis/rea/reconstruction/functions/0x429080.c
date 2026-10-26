// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x429080
// name: FUN_00429080
// ranges: 0x429080-0x42913a
// calls: 0x429080, 0x42a770, 0x42a790

int __fastcall FUN_00429080(int param_1)

{
  uint uVar1;
  
  *(undefined1 *)(param_1 + 0xc) = 0x20;
  *(undefined1 *)(param_1 + 0xd) = 0x20;
  *(undefined1 *)(param_1 + 0xe) = 0x20;
  *(undefined1 *)(param_1 + 0xf) = 0;
  uVar1 = FUN_0042a770(param_1);
  if (uVar1 == 0) {
    uVar1 = FUN_0042a790(param_1);
    if (uVar1 == 0) {
      _sprintf((char *)(param_1 + 0xc),&DAT_00460278,*(undefined4 *)(param_1 + 4));
    }
    else {
      _sprintf((char *)(param_1 + 0xc),&DAT_00460274,*(int *)(param_1 + 4) + 200);
    }
  }
  else {
    _sprintf((char *)(param_1 + 0xc),&DAT_00460270,*(int *)(param_1 + 4) + 100);
  }
  return param_1 + 0xc;
}
