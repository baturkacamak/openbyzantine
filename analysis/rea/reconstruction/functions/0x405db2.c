// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x405db2
// name: FUN_00405db2
// ranges: 0x405db2-0x405e7d
// calls: 0x405db2, 0x44bc20, 0x44c250, 0x44c2f0, 0x44c370

char * __cdecl FUN_00405db2(char *param_1)

{
  FILE *_File;
  int iVar1;
  DWORD DVar2;
  char *pcVar3;
  
  _File = (FILE *)FUN_0044bc20(param_1,&DAT_0045d0b0);
  if (_File != (FILE *)0x0) {
    iVar1 = FUN_0044c250(_File,0,2);
    if (iVar1 == 0) {
      DVar2 = FUN_0044c370((undefined4 *)_File);
      pcVar3 = (char *)FUN_0044c2f0(DVar2);
      FUN_0044c250(_File,0,0);
      __fread_lk(pcVar3,DVar2,1,_File);
      _fclose(_File);
      return pcVar3;
    }
    _fclose(_File);
  }
  return (char *)0x0;
}
