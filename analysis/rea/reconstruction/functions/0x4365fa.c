// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4365fa
// name: FUN_004365fa
// ranges: 0x4365fa-0x436651
// calls: 0x4365fa, 0x44ac40

void FUN_004365fa(void)

{
  int *piVar1;
  int *local_c;
  
  local_c = DAT_0047a89c;
  while (local_c != (int *)0x0) {
    *(undefined1 *)(*local_c + 0x10) = *(undefined1 *)((int)local_c + 7);
    piVar1 = (int *)local_c[2];
    FUN_0044ac40(local_c);
    local_c = piVar1;
  }
  DAT_0047a89c = (int *)0x0;
  return;
}
