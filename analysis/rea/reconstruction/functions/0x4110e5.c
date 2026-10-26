// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4110e5
// name: FUN_004110e5
// ranges: 0x4110e5-0x4111c6
// calls: 0x4110e5, 0x44ba00, 0x44c2f0, 0x44c580

int __cdecl FUN_004110e5(int param_1,uint *param_2,undefined4 param_3)

{
  int iVar1;
  char *pcVar2;
  undefined4 uVar3;
  int local_8;
  
  iVar1 = _strcmp((char *)param_2,&DAT_0045e9e0);
  if (iVar1 == 0) {
    local_8 = -1;
  }
  else {
    for (local_8 = 0; (local_8 < 0x14 && (*(int *)(param_1 + 0x2c + local_8 * 4) != 0));
        local_8 = local_8 + 1) {
      iVar1 = _strcmp(*(char **)(param_1 + 0x2c + local_8 * 4),(char *)param_2);
      if (iVar1 == 0) {
        return local_8;
      }
    }
    pcVar2 = FUN_0044c580(param_2);
    uVar3 = FUN_0044c2f0((SIZE_T)(pcVar2 + 1));
    *(undefined4 *)(param_1 + 0x2c + local_8 * 4) = uVar3;
    FUN_0044ba00(*(uint **)(param_1 + 0x2c + local_8 * 4),param_2);
    *(undefined4 *)(param_1 + 0x7c + local_8 * 4) = param_3;
  }
  return local_8;
}
