// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x43e830
// name: FUN_0043e830
// ranges: 0x43e830-0x43e8a1
// calls: 0x43e830

undefined4 __cdecl FUN_0043e830(int *param_1)

{
  undefined4 *puVar1;
  uint uVar2;
  
  puVar1 = (undefined4 *)param_1[5];
  uVar2 = __fread_lk((char *)puVar1[8],1,0x1000,(FILE *)puVar1[7]);
  if (uVar2 == 0) {
    if (*(char *)(puVar1 + 9) != '\0') {
      *(undefined4 *)(*param_1 + 0x14) = 0x29;
      (**(code **)*param_1)(param_1);
    }
    *(undefined4 *)(*param_1 + 0x14) = 0x74;
    (**(code **)(*param_1 + 4))(param_1,0xffffffff);
    uVar2 = 2;
    *(undefined1 *)puVar1[8] = 0xff;
    *(undefined1 *)(puVar1[8] + 1) = 0xd9;
  }
  *puVar1 = puVar1[8];
  puVar1[1] = uVar2;
  *(undefined1 *)(puVar1 + 9) = 0;
  return CONCAT31((int3)(uVar2 >> 8),1);
}
