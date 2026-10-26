// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x44ce90
// name: _strtok
// ranges: 0x44ce90-0x44cf64
// calls: -

/* Library Function - Single Match
    _strtok
   
   Library: Visual Studio 1998 Release */

char * __cdecl _strtok(char *_Str,char *_Delim)

{
  byte bVar1;
  uint uVar2;
  int iVar3;
  byte *pbVar4;
  byte local_20 [32];
  
  pbVar4 = local_20;
  for (iVar3 = 8; iVar3 != 0; iVar3 = iVar3 + -1) {
    pbVar4[0] = 0;
    pbVar4[1] = 0;
    pbVar4[2] = 0;
    pbVar4[3] = 0;
    pbVar4 = pbVar4 + 4;
  }
  do {
    bVar1 = *_Delim;
    _Delim = _Delim + 1;
    local_20[bVar1 >> 3] = local_20[bVar1 >> 3] | '\x01' << (bVar1 & 7);
  } while (bVar1 != 0);
  if (_Str == (char *)0x0) {
    _Str = (char *)DAT_0046e0dc;
  }
  uVar2 = 1 << (*_Str & 7U) & (uint)local_20[(byte)*_Str >> 3];
  for (; (uVar2 != 0 && (*_Str != 0)); _Str = _Str + 1) {
    uVar2 = 1 << (_Str[1] & 7U) & (uint)local_20[(byte)_Str[1] >> 3];
  }
  bVar1 = *_Str;
  DAT_0046e0dc = (byte *)_Str;
  do {
    if (bVar1 == 0) {
LAB_0044cf49:
      return (char *)((DAT_0046e0dc == (byte *)_Str) - 1 & (uint)_Str);
    }
    if ((1 << (*DAT_0046e0dc & 7) & (uint)local_20[*DAT_0046e0dc >> 3]) != 0) {
      *DAT_0046e0dc = 0;
      DAT_0046e0dc = DAT_0046e0dc + 1;
      goto LAB_0044cf49;
    }
    DAT_0046e0dc = DAT_0046e0dc + 1;
    bVar1 = *DAT_0046e0dc;
  } while( true );
}
