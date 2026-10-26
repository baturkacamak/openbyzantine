// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4422c0
// name: FUN_004422c0
// ranges: 0x4422c0-0x4422da
// calls: 0x4422c0

void __cdecl FUN_004422c0(undefined4 *param_1,uint param_2)

{
  uint uVar1;
  
  for (uVar1 = param_2 >> 2; uVar1 != 0; uVar1 = uVar1 - 1) {
    *param_1 = 0;
    param_1 = param_1 + 1;
  }
  for (uVar1 = param_2 & 3; uVar1 != 0; uVar1 = uVar1 - 1) {
    *(undefined1 *)param_1 = 0;
    param_1 = (undefined4 *)((int)param_1 + 1);
  }
  return;
}
