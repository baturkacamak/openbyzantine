// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x453f10
// name: setSBCS
// ranges: 0x453f10-0x453f3b
// calls: -

/* Library Function - Single Match
    _setSBCS
   
   Library: Visual Studio 1998 Release */

void __cdecl setSBCS(void)

{
  int iVar1;
  undefined4 *puVar2;
  
  puVar2 = &DAT_00463918;
  for (iVar1 = 0x40; iVar1 != 0; iVar1 = iVar1 + -1) {
    *puVar2 = 0;
    puVar2 = puVar2 + 1;
  }
  *(undefined1 *)puVar2 = 0;
  DAT_00463a28 = 0;
  DAT_00463a1c = 0;
  DAT_00463a20 = 0;
  DAT_00463a2c = 0;
  DAT_00463a30 = 0;
  return;
}
