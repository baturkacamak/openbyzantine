// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x40fe70
// name: FUN_0040fe70
// ranges: 0x40fe70-0x40ffed
// calls: 0x40ac38, 0x40ac43, 0x40c624, 0x40fe70, 0x410078, 0x41cb13, 0x44ba00, 0x44c540

DWORD __cdecl FUN_0040fe70(uint param_1,int *param_2)

{
  int iVar1;
  DWORD local_5c;
  uint local_58 [10];
  uint *local_30;
  uint local_2c [10];
  
  _sprintf((char *)local_2c,s__c_c__s_0045e800,
           (uint)*(byte *)**(undefined4 **)(&DAT_0047dd70 + (param_1 & 0xffff) * 4),
           (uint)*(byte *)(**(int **)(&DAT_0047dd70 + (param_1 & 0xffff) * 4) + 1),
           **(undefined4 **)(&DAT_0047dd70 + (param_1 & 0xffff) * 4));
  FUN_0040c624(local_58,(uint *)s_anims_0045e810,local_2c,(uint *)&DAT_0045e808);
  local_5c = FUN_00410078((char *)local_58,param_2);
  if (local_5c == 0) {
    local_30 = (uint *)FUN_0044c540((char *)local_58,&DAT_0045e818);
    FUN_0044ba00(local_30,(uint *)&DAT_0045e820);
    local_5c = FUN_0041cb13((char *)local_58,param_2);
  }
  if (local_5c == 0) {
    FUN_0040ac43();
    FUN_0040ac38();
    local_5c = 0;
  }
  else {
    *(undefined1 *)(*(int *)(&DAT_0047dd70 + (param_1 & 0xffff) * 4) + 9) = 1;
    iVar1 = __strcmpi((char *)**(undefined4 **)(&DAT_0047dd70 + (param_1 & 0xffff) * 4),
                      s_mrmh01a_0045e858);
    if ((iVar1 == 0) ||
       (iVar1 = __strcmpi((char *)**(undefined4 **)(&DAT_0047dd70 + (param_1 & 0xffff) * 4),
                          s_mrmh01b_0045e860), iVar1 == 0)) {
      DAT_0045c05c = 1;
    }
  }
  return local_5c;
}
