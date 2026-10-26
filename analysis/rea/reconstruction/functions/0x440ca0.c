// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x440ca0
// name: FUN_00440ca0
// ranges: 0x440ca0-0x440d4f
// calls: 0x440ca0

uint __cdecl FUN_00440ca0(int *param_1)

{
  byte bVar1;
  undefined4 *puVar2;
  uint uVar3;
  byte *pbVar4;
  int iVar5;
  
  puVar2 = (undefined4 *)param_1[5];
  pbVar4 = (byte *)*puVar2;
  iVar5 = puVar2[1];
  if (iVar5 == 0) {
    uVar3 = (*(code *)puVar2[3])(param_1);
    if ((char)uVar3 == '\0') {
      return uVar3 & 0xffffff00;
    }
    pbVar4 = (byte *)*puVar2;
    iVar5 = puVar2[1];
  }
  iVar5 = iVar5 + -1;
  bVar1 = *pbVar4;
  pbVar4 = pbVar4 + 1;
  if (iVar5 == 0) {
    uVar3 = (*(code *)puVar2[3])(param_1);
    if ((char)uVar3 == '\0') {
      return uVar3 & 0xffffff00;
    }
    pbVar4 = (byte *)*puVar2;
    iVar5 = puVar2[1];
  }
  uVar3 = (uint)*pbVar4;
  if ((bVar1 != 0xff) || (uVar3 != 0xd8)) {
    *(undefined4 *)(*param_1 + 0x14) = 0x34;
    *(uint *)(*param_1 + 0x18) = (uint)bVar1;
    *(uint *)(*param_1 + 0x1c) = uVar3;
    (**(code **)*param_1)(param_1);
  }
  param_1[0x5e] = uVar3;
  *puVar2 = pbVar4 + 1;
  puVar2[1] = iVar5 + -1;
  return 1;
}
