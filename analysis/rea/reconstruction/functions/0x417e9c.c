// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x417e9c
// name: FUN_00417e9c
// ranges: 0x417e9c-0x417fd7
// calls: 0x40bbb0, 0x40bbfd, 0x417e9c

void FUN_00417e9c(void)

{
  int *piVar1;
  undefined4 local_8c [9];
  int local_68;
  int local_20;
  int local_1c;
  int local_18;
  tagRECT local_14;
  
  if (DAT_0045f210 == 0) {
    local_18 = 3;
  }
  else {
    local_18 = 1;
  }
  piVar1 = (int *)(&DAT_0046c520)[local_18];
  if (piVar1 != (int *)0x0) {
    _memset(local_8c,0,0x6c);
    local_8c[0] = 0x6c;
    (**(code **)(*DAT_0045f218 + 100))(DAT_0045f218,0,local_8c,1,0);
    local_1c = (int)*(short *)(piVar1[1] + 6);
    local_20 = (int)*(short *)(piVar1[1] + 8);
    SetRect(&local_14,local_1c,local_20,*(short *)(piVar1[1] + 2) + local_1c,
            *(short *)(piVar1[1] + 4) + local_20);
    FUN_0040bbb0((short)local_1c,(short)local_20,(int)*(short *)(piVar1[1] + 4),0x280);
    FUN_0040bbfd(*piVar1,local_68,(short)local_1c,(short)local_20,&local_14.left,0);
    (**(code **)(*DAT_0045f218 + 0x80))(DAT_0045f218,0);
  }
  return;
}
