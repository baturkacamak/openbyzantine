// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x40ffee
// name: FUN_0040ffee
// ranges: 0x40ffee-0x410077
// calls: 0x40ffee, 0x44c2f0

void __cdecl FUN_0040ffee(void *param_1,undefined2 param_2)

{
  undefined4 uVar1;
  
  _memset(param_1,0,0x40);
  *(undefined2 *)((int)param_1 + 0x14) = param_2;
  *(undefined2 *)((int)param_1 + 2) = 0x14;
  *(undefined1 *)((int)param_1 + 9) = 0x20;
  *(undefined2 *)((int)param_1 + 4) = 10;
  *(undefined4 *)((int)param_1 + 0x2c) = 0xffffffff;
  *(undefined4 *)((int)param_1 + 0x30) = 0xffffffff;
  uVar1 = FUN_0044c2f0(0x18);
  *(undefined4 *)((int)param_1 + 0x18) = uVar1;
  *(undefined4 *)((int)param_1 + 0x1c) = *(undefined4 *)((int)param_1 + 0x18);
  *(undefined4 *)(*(int *)((int)param_1 + 0x18) + 8) = *(undefined4 *)((int)param_1 + 0x18);
  return;
}
