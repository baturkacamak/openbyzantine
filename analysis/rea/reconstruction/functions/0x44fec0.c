// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x44fec0
// name: __handle_qnan1
// ranges: 0x44fec0-0x44ff0e
// calls: 0x4508f0

/* Library Function - Single Match
    __handle_qnan1
   
   Library: Visual Studio 1998 Release */

float10 __cdecl __handle_qnan1(int param_1,undefined4 param_2,undefined4 param_3)

{
  float10 fVar1;
  
  if (DAT_00463ebc == 0) {
    fVar1 = __umatherr(1,param_1);
    return fVar1;
  }
  DAT_00462c74 = 0x21;
  FUN_004508f0();
  return (float10)(double)CONCAT44(param_3,param_2);
}
