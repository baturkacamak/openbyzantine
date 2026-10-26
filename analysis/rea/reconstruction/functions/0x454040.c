// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x454040
// name: __FF_MSGBANNER
// ranges: 0x454040-0x45407c
// calls: 0x454080

/* Library Function - Single Match
    __FF_MSGBANNER
   
   Library: Visual Studio 1998 Release */

void __cdecl __FF_MSGBANNER(void)

{
  if ((DAT_00463190 == 1) || ((DAT_00463190 == 0 && (DAT_00463194 == 1)))) {
    FUN_00454080(0xfc);
    if (DAT_00463e20 != (code *)0x0) {
      (*DAT_00463e20)();
    }
    FUN_00454080(0xff);
  }
  return;
}
