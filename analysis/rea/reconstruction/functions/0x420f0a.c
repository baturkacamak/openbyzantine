// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x420f0a
// name: FUN_00420f0a
// ranges: 0x420f0a-0x420fe4
// calls: 0x416710, 0x418040, 0x4181c5, 0x420f0a

void __cdecl FUN_00420f0a(int param_1)

{
  int *piVar1;
  tagRECT *ptVar2;
  int iVar3;
  int iVar4;
  char local_11c [260];
  int *local_18;
  tagRECT local_14;
  
  local_18 = (int *)0x0;
  local_14.left = 0;
  local_14.top = 0;
  local_14.right = 0;
  local_14.bottom = 0;
  if (param_1 == -1) {
    SetRect(&local_14,0x77,0x38,0x120,0x10f);
    iVar4 = 0x38;
    iVar3 = 0x77;
    ptVar2 = &local_14;
    piVar1 = (int *)FUN_00418040(DAT_0047a8a4);
    FUN_004181c5(piVar1,&ptVar2->left,iVar3,iVar4);
  }
  else {
    _sprintf(local_11c,s_setupart_portrait_photo_02d_tga_0045fc58,param_1);
    FUN_00416710(local_11c,&local_18,1,(uint *)&local_14.right,(uint *)&local_14.bottom);
    if (local_18 != (int *)0x0) {
      FUN_004181c5(local_18,&local_14.left,0x77,0x38);
      (**(code **)(*local_18 + 8))(local_18);
    }
  }
  return;
}
