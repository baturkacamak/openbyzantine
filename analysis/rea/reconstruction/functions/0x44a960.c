// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x44a960
// name: _CallMemberFunction0
// ranges: 0x44a960-0x44a966
// calls: -

/* Library Function - Single Match
    void __stdcall _CallMemberFunction0(void *,void *)
   
   Library: Visual Studio 1998 Release */

void _CallMemberFunction0(void *param_1,void *param_2)

{
  LOCK();
  UNLOCK();
                    /* WARNING: Could not recover jumptable at 0x0044a965. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*param_2)();
  return;
}
