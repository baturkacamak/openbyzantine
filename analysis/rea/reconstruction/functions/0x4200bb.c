// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4200bb
// name: FUN_004200bb
// ranges: 0x4200bb-0x4201cb
// calls: 0x40be80, 0x40cb26, 0x4200bb, 0x44bc20, 0x44c2f0
// port: RLE frame loading [partial]

int __cdecl FUN_004200bb(int param_1,char *param_2)

{
  int *piVar1;
  uint uVar2;
  short sVar3;
  FILE *_File;
  undefined4 uVar4;
  int iVar5;
  int local_10;
  int local_c;
  
  local_c = 0;
  _File = (FILE *)FUN_0044bc20(param_2,&DAT_0045fbdc);
  if (_File != (FILE *)0x0) {
    sVar3 = FUN_0040cb26(_File,(char *)0x0);
    local_c = (int)sVar3;
  }
  for (local_10 = 0; local_10 < local_c; local_10 = local_10 + 1) {
    uVar4 = FUN_0044c2f0(0xc);
    *(undefined4 *)(param_1 + local_10 * 4) = uVar4;
    piVar1 = *(int **)(param_1 + local_10 * 4);
    iVar5 = FUN_0044c2f0(0x1a);
    piVar1[1] = iVar5;
    __fread_lk((char *)piVar1[1],0x1a,1,_File);
    uVar2 = *(uint *)(piVar1[1] + 0x16);
    iVar5 = FUN_0044c2f0(uVar2 * 2);
    *piVar1 = iVar5;
    __fread_lk((char *)*piVar1,uVar2,2,_File);
    FUN_0040be80(*piVar1,(int)*(short *)(piVar1[1] + 4));
  }
  _fclose(_File);
  return local_c;
}
