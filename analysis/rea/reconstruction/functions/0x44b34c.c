// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x44b34c
// name: __adj_fdivr_m32
// ranges: 0x44b34c-0x44b397
// calls: -

/* Library Function - Single Match
    __adj_fdivr_m32
   
   Libraries: Visual Studio 1998 Debug, Visual Studio 1998 Release, Visual Studio 2003 Debug, Visual
   Studio 2003 Release */

undefined4 __adj_fdivr_m32(uint param_1)

{
  undefined4 in_EAX;
  ushort in_FPUStatusWord;
  
  if ((param_1 & 0x7f800000) == 0x7f800000) {
    return in_EAX;
  }
  if ((in_FPUStatusWord & 0x3800) != 0) {
    __fdivrp_sti_st();
    return in_EAX;
  }
  __fdivrp_sti_st();
  return in_EAX;
}
