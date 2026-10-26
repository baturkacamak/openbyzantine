// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x41cb13
// name: FUN_0041cb13
// ranges: 0x41cb13-0x41cd24; 0x41cd3c-0x41cd4a
// calls: 0x40ffee, 0x418a1e, 0x41c2bb, 0x41c704, 0x41cb13, 0x41ce80, 0x44ac60, 0x44c2f0, 0x44c540

undefined4 __cdecl FUN_0041cb13(char *param_1,int *param_2)

{
  char *pcVar1;
  void *this;
  DWORD DVar2;
  undefined4 uVar3;
  int iVar4;
  undefined4 *unaff_FS_OFFSET;
  CSplitterWnd *local_34;
  undefined4 *local_2c;
  uint local_28;
  int local_24;
  undefined4 local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_0041cd32;
  local_10 = *unaff_FS_OFFSET;
  *unaff_FS_OFFSET = &local_10;
  pcVar1 = FUN_0044c540(param_1,&DAT_0045f350);
  if (pcVar1 != (char *)0x0) {
    _sprintf(param_1,s_anims__s_0045f358,pcVar1 + -2);
    FUN_00418a1e();
    DAT_0045d1d0 = 0;
  }
  local_28 = (uint)(pcVar1 != (char *)0x0);
  this = (void *)FUN_0044ac60(0x284);
  local_8 = 0;
  if (this == (void *)0x0) {
    local_34 = (CSplitterWnd *)0x0;
  }
  else {
    local_34 = (CSplitterWnd *)FUN_0041c2bb(this,local_28);
  }
  local_8 = 0xffffffff;
  DVar2 = FUN_0041c704(local_34,param_1);
  if (DVar2 == 0) {
    local_24 = 0;
    while( true ) {
      iVar4 = CSplitterWnd::IsTracking(local_34);
      if (iVar4 <= local_24) break;
      if (*param_2 == 0) {
        iVar4 = FUN_0044c2f0(0x40);
        *param_2 = iVar4;
        iVar4 = CSplitterWnd::IsTracking(local_34);
        FUN_0040ffee((void *)*param_2,(short)iVar4);
        local_2c = *(undefined4 **)(*param_2 + 0x18);
      }
      else {
        uVar3 = FUN_0044c2f0(0x18);
        local_2c[1] = uVar3;
        *(undefined4 **)(local_2c[1] + 8) = local_2c;
        local_2c = (undefined4 *)local_2c[1];
        *(undefined4 **)(*(int *)(*param_2 + 0x18) + 8) = local_2c;
      }
      local_2c[1] = *(undefined4 *)(*param_2 + 0x18);
      iVar4 = CSplitterWnd::IsTracking(local_34);
      local_2c[3] = iVar4;
      iVar4 = CSplitterWnd::IsTracking(local_34);
      local_2c[4] = iVar4;
      local_2c[5] = local_24;
      *local_2c = 0;
      local_24 = local_24 + 1;
    }
    *(CSplitterWnd **)(*param_2 + 0x38) = local_34;
    uVar3 = 1;
  }
  else {
    if (local_34 != (CSplitterWnd *)0x0) {
      FUN_0041ce80(local_34,1);
    }
    uVar3 = 0;
  }
  *unaff_FS_OFFSET = local_10;
  return uVar3;
}
