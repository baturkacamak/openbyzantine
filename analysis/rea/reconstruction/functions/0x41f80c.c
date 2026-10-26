// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x41f80c
// name: FUN_0041f80c
// ranges: 0x41f80c-0x41f90d
// calls: 0x40bbb0, 0x40bbfd, 0x418040, 0x4181c5, 0x41f80c, 0x420683

void __cdecl FUN_0041f80c(int param_1,int param_2,int param_3,int param_4)

{
  short sVar1;
  short sVar2;
  int iVar3;
  int iVar4;
  int *piVar5;
  tagRECT *ptVar6;
  tagRECT local_14;
  
  piVar5 = *(int **)(param_1 + param_2 * 4);
  if (piVar5 != (int *)0x0) {
    sVar1 = *(short *)(piVar5[1] + 6);
    sVar2 = *(short *)(piVar5[1] + 8);
    SetRect(&local_14,(int)sVar1,(int)sVar2,(int)*(short *)(piVar5[1] + 2) + (int)sVar1,
            (int)*(short *)(piVar5[1] + 4) + (int)sVar2);
    if (param_3 == 1) {
      FUN_0040bbb0(sVar1,sVar2,(int)*(short *)(piVar5[1] + 4),0x280);
      FUN_0040bbfd(*piVar5,DAT_00477344,sVar1,sVar2,&local_14.left,0);
    }
    else {
      FUN_00420683(&local_14.left);
    }
    if (param_4 != 0) {
      iVar3 = (int)sVar2;
      iVar4 = (int)sVar1;
      ptVar6 = &local_14;
      piVar5 = (int *)FUN_00418040(DAT_00477344);
      FUN_004181c5(piVar5,&ptVar6->left,iVar4,iVar3);
    }
  }
  return;
}
