// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x447010
// name: FUN_00447010
// ranges: 0x447010-0x447115
// calls: 0x447010, 0x447120, 0x4472f0

void __cdecl FUN_00447010(int param_1,int param_2,int param_3,int param_4)

{
  int iVar1;
  int iVar2;
  int iVar3;
  short *psVar4;
  int *piVar5;
  int iVar6;
  int iVar7;
  byte *pbVar8;
  int local_18c;
  byte local_180 [128];
  undefined1 local_100 [256];
  byte *pbVar9;
  
  iVar6 = *(int *)(*(int *)(param_1 + 0x1a4) + 0x18);
  iVar7 = (param_2 >> 2) * 0x20 + 4;
  iVar1 = (param_3 >> 3) * 0x20 + 2;
  iVar2 = (param_4 >> 2) * 0x20 + 4;
  iVar3 = FUN_00447120(param_1,iVar7,iVar1,iVar2,(int)local_100);
  FUN_004472f0(param_1,iVar7,iVar1,iVar2,iVar3,(int)local_100,local_180);
  pbVar8 = local_180;
  piVar5 = (int *)(iVar6 + (param_2 >> 2) * 0x10);
  local_18c = 4;
  do {
    iVar1 = 8;
    iVar6 = ((param_3 >> 3) * 0x100 + (param_4 >> 2) * 4) * 2;
    do {
      iVar2 = 4;
      psVar4 = (short *)(*piVar5 + iVar6);
      pbVar9 = pbVar8;
      do {
        pbVar8 = pbVar9 + 1;
        iVar2 = iVar2 + -1;
        *psVar4 = *pbVar9 + 1;
        psVar4 = psVar4 + 1;
        pbVar9 = pbVar8;
      } while (iVar2 != 0);
      iVar6 = iVar6 + 0x40;
      iVar1 = iVar1 + -1;
    } while (iVar1 != 0);
    piVar5 = piVar5 + 1;
    local_18c = local_18c + -1;
  } while (local_18c != 0);
  return;
}
