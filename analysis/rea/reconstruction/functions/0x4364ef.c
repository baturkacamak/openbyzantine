// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4364ef
// name: FUN_004364ef
// ranges: 0x4364ef-0x4365b4
// calls: 0x4364ef, 0x44c2f0

void __cdecl FUN_004364ef(int param_1,undefined1 param_2,undefined1 param_3,undefined1 param_4)

{
  int iVar1;
  int *local_8;
  
  if (DAT_0047a89c == (int *)0x0) {
    DAT_0047a89c = (int *)FUN_0044c2f0(0xc);
    local_8 = DAT_0047a89c;
  }
  else {
    for (local_8 = DAT_0047a89c; local_8[2] != 0; local_8 = (int *)local_8[2]) {
    }
    iVar1 = FUN_0044c2f0(0xc);
    local_8[2] = iVar1;
    local_8 = (int *)local_8[2];
  }
  local_8[2] = 0;
  *local_8 = param_1;
  *(undefined1 *)((int)local_8 + 7) = *(undefined1 *)(param_1 + 0x10);
  *(undefined1 *)(local_8 + 1) = param_2;
  *(undefined1 *)((int)local_8 + 5) = param_3;
  *(undefined1 *)((int)local_8 + 6) = param_4;
  return;
}
