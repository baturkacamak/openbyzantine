// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x454c00
// name: FID_conflict:__ld12tod
// ranges: 0x454c00-0x454c17
// calls: -

/* Library Function - Multiple Matches With Different Base Names
    __ld12tod
    __ld12tof
   
   Library: Visual Studio 1998 Release */

INTRNCVT_STATUS __cdecl FID_conflict___ld12tod(_LDBL12 *_Ifp,_CRT_DOUBLE *_D)

{
  INTRNCVT_STATUS IVar1;
  
  IVar1 = __ld12cvt((ushort *)_Ifp,(uint *)_D,(int *)&DAT_00463e88);
  return IVar1;
}
