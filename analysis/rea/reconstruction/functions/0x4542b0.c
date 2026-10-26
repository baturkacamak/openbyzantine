// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4542b0
// name: __mbsnbicoll
// ranges: 0x4542b0-0x4542ea
// calls: 0x457190

/* Library Function - Single Match
    __mbsnbicoll
   
   Library: Visual Studio 1998 Release */

int __cdecl __mbsnbicoll(uchar *_Str1,uchar *_Str2,size_t _MaxCount)

{
  size_t sVar1;
  
  if (_MaxCount == 0) {
    return 0;
  }
  sVar1 = FUN_00457190(DAT_00463a20,1,_Str1,_MaxCount,_Str2,_MaxCount,DAT_00463a1c);
  if (sVar1 == 0) {
    return 0x7fffffff;
  }
  return sVar1 - 2;
}
