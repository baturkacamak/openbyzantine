// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x44ff10
// name: __handle_qnan2
// ranges: 0x44ff10-0x44ff7e
// calls: 0x4508f0

/* Library Function - Single Match
    __handle_qnan2
   
   Library: Visual Studio 1998 Release */

float10 __cdecl
__handle_qnan2(int param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
              undefined4 param_5)

{
  float10 fVar1;
  
  if (DAT_00463ebc == 0) {
    fVar1 = __umatherr(1,param_1);
    return fVar1;
  }
  DAT_00462c74 = 0x21;
  FUN_004508f0();
  return (float10)((double)CONCAT44(param_5,param_4) + (double)CONCAT44(param_3,param_2));
}
