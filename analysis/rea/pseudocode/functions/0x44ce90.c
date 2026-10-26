// rea decompile_all
// name: _strtok
// address: 0x44ce90
// body: available
// range: 0x44ce90-0x44cf64 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


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

