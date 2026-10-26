// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x40c624
// name: FUN_0040c624
// ranges: 0x40c624-0x40c6d5
// calls: 0x40c624, 0x44ba08

void __cdecl FUN_0040c624(uint *param_1,uint *param_2,uint *param_3,uint *param_4)

{
  if ((DAT_0045da74 == 1) || (DAT_0045da74 == 2)) {
    _sprintf((char *)param_1,s__c___d__0045da84,(int)DAT_00486a0e,DAT_0045da78);
  }
  else {
    _sprintf((char *)param_1,s__c__s_0045da90,(int)DAT_00486a0e,&DAT_0045da8c);
  }
  if (param_2 != (uint *)0x0) {
    FUN_0044ba08(param_1,param_2);
    FUN_0044ba08(param_1,(uint *)&DAT_0045da98);
  }
  FUN_0044ba08(param_1,param_3);
  FUN_0044ba08(param_1,param_4);
  return;
}
