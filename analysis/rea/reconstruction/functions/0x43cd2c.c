// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x43cd2c
// name: FUN_0043cd2c
// ranges: 0x43cd2c-0x43cd9b
// calls: 0x432be2, 0x43ccae, 0x43cd2c

void __cdecl FUN_0043cd2c(int param_1,undefined4 *param_2)

{
  uint uVar1;
  int *local_8;
  
  local_8 = *(int **)(param_1 + 0x34);
  while( true ) {
    if (local_8 == (int *)0x0) {
      return;
    }
    uVar1 = FUN_0043ccae(param_1);
    if (uVar1 == local_8[1]) break;
    local_8 = (int *)local_8[2];
  }
  *param_2 = *(undefined4 *)(&DAT_004654e0 + *local_8 * 4);
  FUN_00432be2(param_1,local_8[1]);
  return;
}
