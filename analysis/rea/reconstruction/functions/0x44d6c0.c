// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x44d6c0
// name: __amsg_exit
// ranges: 0x44d6c0-0x44d6e9
// calls: 0x454080

/* Library Function - Single Match
    __amsg_exit
   
   Library: Visual Studio 1998 Release */

void __cdecl __amsg_exit(int param_1)

{
  if (DAT_00463190 == 1) {
    __FF_MSGBANNER();
  }
  FUN_00454080(param_1);
  (*(code *)PTR___exit_0046318c)(0xff);
  return;
}
