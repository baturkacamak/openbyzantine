// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x454e20
// name: __fltout
// ranges: 0x454e20-0x454e86
// calls: 0x454e90, 0x458190

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* Library Function - Single Match
    __fltout
   
   Library: Visual Studio 1998 Release */

undefined * __fltout(void)

{
  uint local_c;
  uint local_8;
  ushort local_4;
  
  FUN_00454e90(&local_c,(uint *)&stack0x00000004);
  _DAT_0046e230 = FUN_00458190(local_c,local_8,local_4,0x11,0,&DAT_0046e208);
  _DAT_0046e234 = &DAT_0046e20c;
  _DAT_0046e228 = (int)DAT_0046e20a;
  _DAT_0046e22c = (int)DAT_0046e208;
  return &DAT_0046e228;
}
