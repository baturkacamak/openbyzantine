// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x437bcc
// name: FUN_00437bcc
// ranges: 0x437bcc-0x437c71
// calls: 0x4061e7, 0x437bcc

undefined4 __cdecl FUN_00437bcc(int param_1)

{
  char *pcVar1;
  uint in_EAX;
  undefined2 uVar3;
  uint uVar2;
  undefined4 local_10;
  undefined4 local_c;
  
  local_10 = 0;
  local_c = 0;
  do {
    uVar3 = (undefined2)(in_EAX >> 0x10);
    if (0x2ed < local_c) {
LAB_00437c64:
      return CONCAT22(uVar3,(undefined2)local_10);
    }
    pcVar1 = *(char **)(param_1 + 0x720 + local_c * 4);
    if ((pcVar1 == (char *)0x0) || (*pcVar1 == -1)) {
      uVar3 = (undefined2)((uint)pcVar1 >> 0x10);
      goto LAB_00437c64;
    }
    uVar2 = FUN_004061e7(*(int *)(param_1 + 0x720 + local_c * 4),0);
    in_EAX = local_10;
    if (local_10 <= (uVar2 & 0xffff)) {
      in_EAX = uVar2 & 0xffff;
    }
    local_c = local_c + 1;
    local_10 = in_EAX;
  } while( true );
}
