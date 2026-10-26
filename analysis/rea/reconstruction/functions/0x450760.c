// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x450760
// name: __umatherr
// ranges: 0x450760-0x45080d
// calls: 0x4508f0, 0x455900

/* Library Function - Single Match
    __umatherr
   
   Library: Visual Studio 1998 Release */

float10 __cdecl __umatherr(int param_1,int param_2)

{
  int iVar1;
  undefined4 in_stack_0000001c;
  undefined4 in_stack_00000020;
  
  iVar1 = __get_fname(param_2);
  if (iVar1 != 0) {
    FUN_004508f0();
    iVar1 = FUN_00455900();
    if (iVar1 == 0) {
      __set_errno(param_1);
    }
    return (float10)(double)CONCAT44(in_stack_00000020,in_stack_0000001c);
  }
  FUN_004508f0();
  __set_errno(param_1);
  return (float10)(double)CONCAT44(in_stack_00000020,in_stack_0000001c);
}
