// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x44d070
// name: _rand
// ranges: 0x44d070-0x44d099
// calls: -

/* Library Function - Single Match
    _rand
   
   Library: Visual Studio 1998 Release */

int __cdecl _rand(void)

{
  DAT_00462ef8 = DAT_00462ef8 * 0x343fd + 0x269ec3;
  return (DAT_00462ef8 & 0x7fff0000) >> 0x10;
}
