// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x44e870
// name: __setdefaultprecision
// ranges: 0x44e870-0x44e882
// calls: -

/* Library Function - Single Match
    __setdefaultprecision
   
   Library: Visual Studio 1998 Release */

void __setdefaultprecision(void)

{
  __controlfp(0x10000,0x30000);
  return;
}
