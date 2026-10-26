// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x41045b
// name: FUN_0041045b
// ranges: 0x41045b-0x410606
// calls: 0x40860e, 0x40ac38, 0x40ac43, 0x40c624, 0x40f135, 0x41045b, 0x44bc20
// port: Background loading [partial]

bool __cdecl FUN_0041045b(int param_1,uint param_2,int param_3)

{
  int iVar1;
  FILE *_File;
  ushort *puVar2;
  bool bVar3;
  uint local_88 [15];
  int local_4c [3];
  uint local_40 [15];
  
  if (param_3 == 4) {
    iVar1 = FUN_0040860e((char *)local_88);
    if (iVar1 == -1) {
      return true;
    }
    FUN_0040c624(local_40,(uint *)s_inventor_examine_0045e874,local_88,(uint *)&DAT_0045e86c);
    FUN_0044bc20((char *)local_40,&DAT_0045e888);
  }
  else {
    _sprintf((char *)local_88,s__c_c__s_0045e88c,
             (uint)*(byte *)**(undefined4 **)(&DAT_00484e80 + (param_2 & 0xffff) * 4),
             (uint)*(byte *)(**(int **)(&DAT_00484e80 + (param_2 & 0xffff) * 4) + 1),
             **(undefined4 **)(&DAT_00484e80 + (param_2 & 0xffff) * 4));
    FUN_0040c624(local_40,(uint *)s_backs_0045e89c,local_88,(uint *)&DAT_0045e894);
  }
  _File = (FILE *)FUN_0044bc20((char *)local_40,&DAT_0045e8a4);
  if (_File == (FILE *)0x0) {
    FUN_0040ac43();
    FUN_0040ac38();
    bVar3 = true;
  }
  else {
    _fclose(_File);
    local_4c[0] = 0x280;
    local_4c[1] = 400;
    puVar2 = FUN_0040f135((char *)local_40,local_4c,(uint *)(local_4c + 1),
                          (ushort *)(param_1 + 0x6400),0x280);
    bVar3 = puVar2 == (ushort *)0x0;
  }
  return bVar3;
}
