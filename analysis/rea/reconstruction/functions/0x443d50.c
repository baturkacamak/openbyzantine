// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x443d50
// name: FUN_00443d50
// ranges: 0x443d50-0x443e63
// calls: 0x443c50, 0x443d50

uint __cdecl FUN_00443d50(undefined4 *param_1,uint param_2,int param_3,int param_4,int param_5)

{
  undefined4 uVar1;
  int iVar2;
  int *piVar3;
  uint uVar4;
  int local_4;
  
  local_4 = param_5;
  if (param_3 < param_5) {
    uVar1 = FUN_00443c50(param_1,param_2,param_3,param_5);
    if ((char)uVar1 == '\0') {
      return 0xffffffff;
    }
    param_2 = param_1[3];
    param_3 = param_1[4];
  }
  iVar2 = param_3 - param_5;
  uVar4 = (int)param_2 >> ((byte)iVar2 & 0x1f) & (1 << ((byte)param_5 & 0x1f)) - 1U;
  piVar3 = (int *)(param_4 + 0x44 + param_5 * 4);
  if (*piVar3 < (int)uVar4) {
    do {
      if (iVar2 < 1) {
        uVar1 = FUN_00443c50(param_1,param_2,iVar2,1);
        if ((char)uVar1 == '\0') {
          return 0xffffffff;
        }
        param_2 = param_1[3];
        iVar2 = param_1[4];
      }
      iVar2 = iVar2 + -1;
      piVar3 = piVar3 + 1;
      local_4 = local_4 + 1;
      uVar4 = uVar4 * 2 | (int)param_2 >> ((byte)iVar2 & 0x1f) & 1U;
    } while (*piVar3 < (int)uVar4);
  }
  param_1[3] = param_2;
  param_1[4] = iVar2;
  if (local_4 < 0x11) {
    piVar3 = (int *)(param_4 + local_4 * 4);
    return (uint)*(byte *)(((*(int *)(param_4 + 0xd0) + piVar3[0x23]) - *piVar3) + 0x11 + uVar4);
  }
  *(undefined4 *)(*(int *)param_1[5] + 0x14) = 0x72;
  (**(code **)(*(int *)param_1[5] + 4))((int *)param_1[5],0xffffffff);
  return 0;
}
