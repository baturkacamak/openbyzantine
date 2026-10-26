// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x454d50
// name: FID_conflict:__atodbl
// ranges: 0x454d50-0x454d84
// calls: 0x4579f0

/* Library Function - Multiple Matches With Different Base Names
    __atodbl
    __atoflt
   
   Library: Visual Studio 1998 Release */

int __cdecl FID_conflict___atodbl(_CRT_FLOAT *_Result,char *_Str)

{
  INTRNCVT_STATUS IVar1;
  int local_10;
  _LDBL12 local_c;
  
  FUN_004579f0((ushort *)&local_c,&local_10,(byte *)_Str,0,0,0,0);
  IVar1 = FID_conflict___ld12tod(&local_c,(_CRT_DOUBLE *)_Result);
  return IVar1;
}
