// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x44b318
// name: __adj_fdiv_m32i
// ranges: 0x44b318-0x44b34b
// calls: -

/* Library Function - Single Match
    __adj_fdiv_m32i
   
   Libraries: Visual Studio 1998 Debug, Visual Studio 1998 Release, Visual Studio 2003 Debug, Visual
   Studio 2003 Release */

undefined4 __adj_fdiv_m32i(void)

{
  undefined4 in_EAX;
  ushort in_FPUStatusWord;
  
  if ((in_FPUStatusWord & 0x3800) != 0) {
    __fdivp_sti_st();
    return in_EAX;
  }
  __fdivp_sti_st();
  return in_EAX;
}
