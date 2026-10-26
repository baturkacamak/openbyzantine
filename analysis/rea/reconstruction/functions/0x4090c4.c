// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4090c4
// name: FUN_004090c4
// ranges: 0x4090c4-0x4091e4
// calls: 0x4090c4, 0x40bbb0, 0x40bbfd, 0x4181c5

void __cdecl FUN_004090c4(int param_1,int param_2)

{
  short sVar1;
  short sVar2;
  int xLeft;
  int yTop;
  int *local_20;
  tagRECT local_14;
  
  if (param_1 < 5) {
    local_20 = *(int **)(&DAT_00468ce8 + param_1 * 4);
  }
  else {
    local_20 = *(int **)(&DAT_00468d60 + (param_1 + -5) * 4);
  }
  if (local_20 != (int *)0x0) {
    sVar1 = *(short *)(local_20[1] + 6);
    xLeft = (int)sVar1;
    sVar2 = *(short *)(local_20[1] + 8);
    yTop = (int)sVar2;
    SetRect(&local_14,xLeft,yTop,*(short *)(local_20[1] + 2) + xLeft,
            *(short *)(local_20[1] + 4) + yTop);
    if (param_2 == 0) {
      (**(code **)(*DAT_00486888 + 0x1c))(DAT_00486888,xLeft,yTop,DAT_0045d1b8,&local_14,0);
    }
    else {
      FUN_0040bbb0(sVar1,sVar2,(int)*(short *)(local_20[1] + 4),0x280);
      FUN_0040bbfd(*local_20,DAT_0046e82c,sVar1,sVar2,&local_14.left,0);
    }
    FUN_004181c5(DAT_00486888,&local_14.left,xLeft,yTop);
  }
  return;
}
