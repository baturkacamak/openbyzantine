// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4551a0
// name: _abort
// ranges: 0x4551a0-0x4551be
// calls: 0x454080

/* Library Function - Single Match
    _abort
   
   Library: Visual Studio 1998 Release */

void __cdecl _abort(void)

{
  FUN_00454080(10);
  _raise(0x16);
  __exit(3);
  return;
}
