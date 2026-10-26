// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x447c10
// name: FUN_00447c10
// ranges: 0x447c10-0x447d64
// calls: 0x447850, 0x447c10, 0x447d70, 0x447da0

int __cdecl FUN_00447c10(int param_1,int param_2,int param_3,int param_4)

{
  char cVar1;
  int *piVar2;
  int iVar3;
  int *piVar4;
  int local_18;
  int local_8;
  
  if (param_3 < param_4) {
    local_8 = param_3 * 2;
    piVar4 = (int *)(param_2 + param_3 * 0x20);
    do {
      if (param_4 < local_8) {
        piVar2 = (int *)FUN_00447da0(param_2,param_3);
      }
      else {
        piVar2 = (int *)FUN_00447d70(param_2,param_3);
      }
      if (piVar2 == (int *)0x0) {
        return param_3;
      }
      piVar4[1] = piVar2[1];
      piVar4[3] = piVar2[3];
      piVar4[5] = piVar2[5];
      *piVar4 = *piVar2;
      piVar4[2] = piVar2[2];
      piVar4[4] = piVar2[4];
      iVar3 = (piVar2[1] - *piVar2) * 0x10;
      local_18 = (piVar2[3] - piVar2[2]) * 0xc;
      cVar1 = iVar3 <= local_18;
      if (!(bool)cVar1) {
        local_18 = iVar3;
      }
      if (local_18 < (piVar2[5] - piVar2[4]) * 8) {
        cVar1 = '\x02';
      }
      if (cVar1 == '\0') {
        iVar3 = (piVar2[1] + *piVar2) / 2;
        piVar2[1] = iVar3;
        *piVar4 = iVar3 + 1;
      }
      else if (cVar1 == '\x01') {
        iVar3 = (piVar2[3] + piVar2[2]) / 2;
        piVar2[3] = iVar3;
        piVar4[2] = iVar3 + 1;
      }
      else if (cVar1 == '\x02') {
        iVar3 = (piVar2[5] + piVar2[4]) / 2;
        piVar2[5] = iVar3;
        piVar4[4] = iVar3 + 1;
      }
      param_3 = param_3 + 1;
      FUN_00447850(param_1,piVar2);
      FUN_00447850(param_1,piVar4);
      local_8 = local_8 + 2;
      piVar4 = piVar4 + 8;
    } while (param_3 < param_4);
  }
  return param_3;
}
