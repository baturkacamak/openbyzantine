// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x44a920
// name: _JumpToContinuation
// ranges: 0x44a920-0x44a94d
// calls: -

/* Library Function - Single Match
    void __stdcall _JumpToContinuation(void *,struct EHRegistrationNode *)
   
   Library: Visual Studio 1998 Release */

void _JumpToContinuation(void *param_1,EHRegistrationNode *param_2)

{
  undefined4 *unaff_FS_OFFSET;
  
  *unaff_FS_OFFSET = *(undefined4 *)*unaff_FS_OFFSET;
                    /* WARNING: Could not recover jumptable at 0x0044a94c. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*param_1)();
  return;
}
