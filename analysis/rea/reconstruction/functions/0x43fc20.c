// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x43fc20
// name: FUN_0043fc20
// ranges: 0x43fc20-0x43fcca
// calls: 0x43fc20

void __cdecl FUN_0043fc20(int *param_1)

{
  int iVar1;
  int iVar2;
  
  *(undefined4 *)(*param_1 + 0x14) = 0x65;
  (**(code **)(*param_1 + 4))(param_1,1);
  if (*(char *)(param_1[100] + 0x50) != '\0') {
    *(undefined4 *)(*param_1 + 0x14) = 0x3c;
    (**(code **)*param_1)(param_1);
  }
  iVar1 = 0;
  do {
    *(undefined1 *)((int)param_1 + iVar1 + 0xca) = 0;
    iVar2 = iVar1 + 1;
    *(undefined1 *)((int)param_1 + iVar1 + 0xda) = 1;
    *(undefined1 *)((int)param_1 + iVar1 + 0xea) = 5;
    iVar1 = iVar2;
  } while (iVar2 < 0x10);
  param_1[0x3f] = 0;
  param_1[9] = 0;
  *(undefined2 *)((int)param_1 + 0x102) = 1;
  *(undefined1 *)(param_1 + 0x42) = 0;
  *(undefined2 *)(param_1 + 0x41) = 1;
  *(undefined1 *)(param_1 + 0x40) = 0;
  *(undefined1 *)((int)param_1 + 0x101) = 0;
  *(undefined1 *)((int)param_1 + 0x106) = 0;
  *(undefined1 *)((int)param_1 + 0x107) = 0;
  *(undefined1 *)(param_1[100] + 0x50) = 1;
  return;
}
