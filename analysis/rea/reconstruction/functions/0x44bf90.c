// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x44bf90
// name: FID_conflict:_fwprintf
// ranges: 0x44bf90-0x44bfc6
// calls: 0x44db10

/* Library Function - Multiple Matches With Different Base Names
    _fprintf
    _fwprintf
   
   Library: Visual Studio 1998 Release */

int __cdecl FID_conflict__fwprintf(FILE *_File,wchar_t *_Format,...)

{
  int _Flag;
  int iVar1;
  
  _Flag = __stbuf(_File);
  iVar1 = FUN_0044db10(_File,(byte *)_Format,(undefined4 *)&stack0x0000000c);
  __ftbuf(_Flag,_File);
  return iVar1;
}
