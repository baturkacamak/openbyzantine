// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x451740
// name: comexecmd
// ranges: 0x451740-0x4517ae
// calls: 0x44ac40, 0x455d90, 0x455fe0

/* Library Function - Single Match
    _comexecmd
   
   Library: Visual Studio 1998 Release */

HANDLE __cdecl comexecmd(int param_1,LPCSTR param_2,int *param_3,int *param_4)

{
  int iVar1;
  HANDLE pvVar2;
  char *local_8;
  LPVOID local_4;
  
  iVar1 = FUN_00455fe0(param_3,param_4,(int *)&local_8,(int *)&local_4,param_2);
  if (iVar1 == -1) {
    return (HANDLE)0xffffffff;
  }
  pvVar2 = FUN_00455d90(param_1,param_2,local_8,local_4);
  FUN_0044ac40(local_8);
  FUN_0044ac40(local_4);
  return pvVar2;
}
