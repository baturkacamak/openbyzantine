// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x441840
// name: FUN_00441840
// ranges: 0x441840-0x4418cc
// calls: 0x441840

void __cdecl FUN_00441840(undefined4 param_1,int *param_2,char param_3)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  
  iVar3 = 0;
  iVar1 = param_2[2];
  iVar5 = param_2[6] * iVar1 * 0x80;
  if (0 < param_2[4]) {
    do {
      iVar4 = param_2[4] - iVar3;
      if (param_2[5] <= param_2[4] - iVar3) {
        iVar4 = param_2[5];
      }
      iVar2 = param_2[7] - (param_2[6] + iVar3);
      if (iVar2 <= iVar4) {
        iVar4 = iVar2;
      }
      iVar2 = param_2[1] - (param_2[6] + iVar3);
      if (iVar2 <= iVar4) {
        iVar4 = iVar2;
      }
      if (iVar4 < 1) {
        return;
      }
      iVar4 = iVar4 * iVar1 * 0x80;
      if (param_3 == '\0') {
        (*(code *)param_2[10])
                  (param_1,param_2 + 10,*(undefined4 *)(*param_2 + iVar3 * 4),iVar5,iVar4);
      }
      else {
        (*(code *)param_2[0xb])(param_1);
      }
      iVar5 = iVar5 + iVar4;
      iVar3 = iVar3 + param_2[5];
    } while (iVar3 < param_2[4]);
  }
  return;
}
