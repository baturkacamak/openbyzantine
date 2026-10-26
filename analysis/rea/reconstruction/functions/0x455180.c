// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x455180
// name: _ValidateExecute
// ranges: 0x455180-0x45519a
// calls: -

/* Library Function - Single Match
    int __cdecl _ValidateExecute(int (__stdcall*)(void))
   
   Library: Visual Studio 1998 Release */

int __cdecl _ValidateExecute(_func_int *param_1)

{
  BOOL BVar1;
  
  BVar1 = IsBadCodePtr((FARPROC)param_1);
  return (uint)(BVar1 == 0);
}
