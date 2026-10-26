// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x417314
// name: FUN_00417314
// ranges: 0x417314-0x417490
// calls: 0x40bbb0, 0x40bbfd, 0x417314

void __cdecl FUN_00417314(int param_1)

{
  int *piVar1;
  undefined4 local_88 [9];
  int local_64;
  int local_1c;
  int local_18;
  tagRECT local_14;
  
  piVar1 = DAT_0046c5b8;
  if (DAT_0046c5b8 != (int *)0x0) {
    _memset(local_88,0,0x6c);
    local_88[0] = 0x6c;
    local_18 = (int)*(short *)(piVar1[1] + 6);
    local_1c = (int)*(short *)(piVar1[1] + 8);
    SetRect(&local_14,local_18,local_1c,*(short *)(piVar1[1] + 2) + local_18,
            *(short *)(piVar1[1] + 4) + local_1c);
    (**(code **)(*DAT_0045f218 + 100))(DAT_0045f218,0,local_88,1,0);
    if (param_1 == 0) {
      FID_conflict__memcpy
                ((void *)(local_1c * 0x500 + local_64),
                 (void *)(((local_1c - *(short *)(piVar1[1] + 4)) * 5 + -10) * 0x100 + local_64),
                 *(short *)(piVar1[1] + 4) * 0x500);
    }
    else {
      FUN_0040bbb0((short)local_18,(short)local_1c,(int)*(short *)(piVar1[1] + 4),0x280);
      FUN_0040bbfd(*piVar1,local_64,(short)local_18,(short)local_1c,&local_14.left,0);
    }
    (**(code **)(*DAT_0045f218 + 0x80))(DAT_0045f218,0);
  }
  return;
}
