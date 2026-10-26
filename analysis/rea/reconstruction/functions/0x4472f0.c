// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4472f0
// name: FUN_004472f0
// ranges: 0x4472f0-0x447444
// calls: 0x4472f0

void __cdecl
FUN_004472f0(int param_1,int param_2,int param_3,int param_4,int param_5,int param_6,byte *param_7)

{
  byte bVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  uint uVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  int *piVar9;
  byte *pbVar10;
  int local_220;
  int local_21c;
  int local_218;
  int local_214;
  int local_208;
  int local_204;
  int local_200 [128];
  
  piVar9 = local_200;
  for (iVar3 = 0x80; iVar3 != 0; iVar3 = iVar3 + -1) {
    *piVar9 = 0x7fffffff;
    piVar9 = piVar9 + 1;
  }
  local_220 = 0;
  if (0 < param_5) {
    do {
      bVar1 = *(byte *)(local_220 + param_6);
      uVar5 = (uint)bVar1;
      piVar9 = *(int **)(param_1 + 0x74);
      iVar3 = (param_2 - (uint)*(byte *)(*piVar9 + uVar5)) * 2;
      iVar7 = param_3 - (uint)*(byte *)(piVar9[1] + uVar5);
      iVar4 = iVar7 * 3;
      iVar8 = param_4 - (uint)*(byte *)(piVar9[2] + uVar5);
      local_218 = iVar3 * iVar3 + iVar8 * iVar8 + iVar4 * iVar4;
      piVar9 = local_200;
      local_204 = 3;
      pbVar10 = param_7;
      local_214 = (iVar3 + 8) * 0x20;
      do {
        local_208 = 7;
        local_21c = local_218;
        iVar3 = (iVar7 * 9 + 0x12) * 8;
        do {
          iVar2 = 3;
          iVar4 = local_21c;
          iVar6 = (iVar8 + 4) * 0x10;
          do {
            if (iVar4 < *piVar9) {
              *piVar9 = iVar4;
              *pbVar10 = bVar1;
            }
            iVar4 = iVar4 + iVar6;
            piVar9 = piVar9 + 1;
            iVar6 = iVar6 + 0x80;
            pbVar10 = pbVar10 + 1;
            iVar2 = iVar2 + -1;
          } while (-1 < iVar2);
          local_21c = local_21c + iVar3;
          iVar3 = iVar3 + 0x120;
          local_208 = local_208 + -1;
        } while (-1 < local_208);
        local_218 = local_218 + local_214;
        local_204 = local_204 + -1;
        local_214 = local_214 + 0x200;
      } while (-1 < local_204);
      local_220 = local_220 + 1;
    } while (local_220 < param_5);
  }
  return;
}
