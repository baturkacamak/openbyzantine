// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x41159f
// name: FUN_0041159f
// ranges: 0x41159f-0x4117a5
// calls: 0x40ac38, 0x40ac43, 0x40c624, 0x41159f, 0x41199e, 0x44bc20, 0x44c2f0

DWORD __cdecl FUN_0041159f(uint *param_1,undefined4 *param_2,char param_3)

{
  char *pcVar1;
  void *_Dst;
  FILE *_File;
  int iVar2;
  undefined4 uVar3;
  DWORD local_11c;
  undefined1 local_10c;
  uint local_108 [65];
  
  FUN_0040c624(local_108,(uint *)s_sounds_0045ea20,param_1,(uint *)&DAT_0045ea18);
  _File = (FILE *)FUN_0044bc20((char *)local_108,&DAT_0045ea28);
  if ((_File == (FILE *)0x0) && (DAT_00485c6c == '\0')) {
    FUN_0040ac43();
    FUN_0040ac38();
    local_11c = 0;
  }
  else {
    local_11c = FUN_0041199e(_File);
    iVar2 = local_11c + 0xc;
    local_10c = (undefined1)iVar2;
    DAT_0045e7cb = local_10c;
    local_10c = (undefined1)((uint)iVar2 >> 8);
    DAT_0045e7cc = local_10c;
    local_10c = (undefined1)((uint)iVar2 >> 0x10);
    DAT_0045e7cd = local_10c;
    if (param_3 == '\b') {
      DAT_0045e7d2 = 8;
      DAT_0045e7d4 = 0;
    }
    else {
      DAT_0045e7d2 = 0x10;
      DAT_0045e7d4 = 4;
    }
    DAT_0045e7ce = 0x22;
    DAT_0045e7cf = 0x56;
    uVar3 = FUN_0044c2f0(local_11c + 0x2b);
    *param_2 = uVar3;
    _Dst = (void *)*param_2;
    FID_conflict__memcpy(_Dst,&DAT_0045e7b0,0x2a);
    pcVar1 = (char *)((int)_Dst + 0x2a);
    __fread_lk(pcVar1,local_11c,1,_File);
    pcVar1[local_11c] = '\0';
    __fread_lk((char *)*param_2,local_11c,1,_File);
    _fclose(_File);
  }
  return local_11c;
}
