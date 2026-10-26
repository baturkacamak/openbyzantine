// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4477c0
// name: FUN_004477c0
// ranges: 0x4477c0-0x44784c
// calls: 0x4477c0, 0x447850, 0x447c10, 0x447dd0

void __cdecl FUN_004477c0(int *param_1,int param_2)

{
  int *piVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  
  piVar1 = (int *)(**(code **)param_1[1])(param_1,1,param_2 << 5);
  *piVar1 = 0;
  piVar1[1] = 0x1f;
  piVar1[2] = 0;
  piVar1[3] = 0x3f;
  piVar1[4] = 0;
  piVar1[5] = 0x1f;
  FUN_00447850((int)param_1,piVar1);
  iVar2 = FUN_00447c10((int)param_1,(int)piVar1,1,param_2);
  iVar3 = 0;
  if (0 < iVar2) {
    do {
      iVar4 = iVar3 + 1;
      FUN_00447dd0((int)param_1,piVar1,iVar3);
      iVar3 = iVar4;
      piVar1 = piVar1 + 8;
    } while (iVar4 < iVar2);
  }
  param_1[0x1c] = iVar2;
  *(undefined4 *)(*param_1 + 0x14) = 0x5f;
  *(int *)(*param_1 + 0x18) = iVar2;
  (**(code **)(*param_1 + 4))(param_1,1);
  return;
}
