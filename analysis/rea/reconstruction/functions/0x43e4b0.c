// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x43e4b0
// name: FUN_0043e4b0
// ranges: 0x43e4b0-0x43e55d
// calls: 0x43e4b0

uint __cdecl FUN_0043e4b0(int param_1)

{
  uint *puVar1;
  uint uVar2;
  uint uVar3;
  int iVar4;
  
  if (*(int *)(param_1 + 0x10) != 0xcc) {
    (*(code *)**(undefined4 **)(param_1 + 0x17c))(param_1);
    *(undefined4 *)(param_1 + 0x78) = 0;
    *(undefined4 *)(param_1 + 0x10) = 0xcc;
  }
  do {
    if (*(char *)(*(int *)(param_1 + 0x17c) + 8) == '\0') {
      iVar4 = 0xce - (uint)(*(char *)(param_1 + 0x41) == '\0');
      *(int *)(param_1 + 0x10) = iVar4;
      return CONCAT31((int3)((uint)iVar4 >> 8),1);
    }
    puVar1 = (uint *)(param_1 + 0x78);
    while (*puVar1 < *(uint *)(param_1 + 0x60)) {
      if (*(int *)(param_1 + 8) != 0) {
        *(uint *)(*(int *)(param_1 + 8) + 4) = *puVar1;
        *(undefined4 *)(*(int *)(param_1 + 8) + 8) = *(undefined4 *)(param_1 + 0x60);
        (*(code *)**(undefined4 **)(param_1 + 8))(param_1);
      }
      uVar2 = *puVar1;
      uVar3 = (**(code **)(*(int *)(param_1 + 0x180) + 4))(param_1,0,puVar1,0);
      if (*puVar1 == uVar2) {
        return uVar3 & 0xffffff00;
      }
    }
    (**(code **)(*(int *)(param_1 + 0x17c) + 4))(param_1);
    (*(code *)**(undefined4 **)(param_1 + 0x17c))(param_1);
    *puVar1 = 0;
  } while( true );
}
