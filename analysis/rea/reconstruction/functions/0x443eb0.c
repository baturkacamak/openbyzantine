// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x443eb0
// name: FUN_00443eb0
// ranges: 0x443eb0-0x443ff0
// calls: 0x443a70, 0x443eb0

void __cdecl FUN_00443eb0(int *param_1)

{
  int iVar1;
  int iVar2;
  int iVar3;
  byte *pbVar4;
  undefined4 *local_c;
  int *local_8;
  int local_4;
  
  iVar1 = param_1[0x65];
  if ((((param_1[0x5a] != 0) || (param_1[0x5b] != 0x3f)) || (param_1[0x5c] != 0)) ||
     (param_1[0x5d] != 0)) {
    *(undefined4 *)(*param_1 + 0x14) = 0x76;
    (**(code **)(*param_1 + 4))(param_1,0xffffffff);
  }
  local_4 = 0;
  if (0 < param_1[0x48]) {
    local_c = (undefined4 *)(iVar1 + 0x14);
    local_8 = param_1 + 0x49;
    do {
      iVar2 = *(int *)(*local_8 + 0x14);
      iVar3 = *(int *)(*local_8 + 0x18);
      if (((iVar2 < 0) || (3 < iVar2)) || (param_1[iVar2 + 0x28] == 0)) {
        *(undefined4 *)(*param_1 + 0x14) = 0x31;
        *(int *)(*param_1 + 0x18) = iVar2;
        (**(code **)*param_1)(param_1);
      }
      if (((iVar3 < 0) || (3 < iVar3)) || (param_1[iVar3 + 0x2c] == 0)) {
        *(undefined4 *)(*param_1 + 0x14) = 0x31;
        *(int *)(*param_1 + 0x18) = iVar3;
        (**(code **)*param_1)(param_1);
      }
      pbVar4 = FUN_00443a70((int)param_1,param_1[iVar2 + 0x28],(int *)(iVar1 + iVar2 * 4 + 0x28));
      FUN_00443a70((int)param_1,param_1[(int)(pbVar4 + 0x2c)],
                   (int *)(iVar1 + (int)pbVar4 * 4 + 0x38));
      local_4 = local_4 + 1;
      *local_c = 0;
      local_c = local_c + 1;
      local_8 = local_8 + 1;
    } while (local_4 < param_1[0x48]);
  }
  *(undefined4 *)(iVar1 + 0xc) = 0;
  *(undefined4 *)(iVar1 + 8) = 0;
  *(undefined1 *)(iVar1 + 0x10) = 0;
  *(int *)(iVar1 + 0x24) = param_1[0x3f];
  return;
}
