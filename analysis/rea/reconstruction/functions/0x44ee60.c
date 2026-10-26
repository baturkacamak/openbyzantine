// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x44ee60
// name: __cftof_g
// ranges: 0x44ee60-0x44ee85
// calls: -

/* Library Function - Single Match
    __cftof_g
   
   Library: Visual Studio 1998 Release */

void __cdecl __cftof_g(double *param_1,char *param_2,size_t param_3)

{
  int unaff_retaddr;
  
  DAT_00463238 = 1;
  __cftof(param_1,param_2,param_3,unaff_retaddr);
  DAT_00463238 = 0;
  return;
}
