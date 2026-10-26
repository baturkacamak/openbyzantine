// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x450840
// name: __get_fname
// ranges: 0x450840-0x450865
// calls: -

/* Library Function - Single Match
    __get_fname
   
   Library: Visual Studio 1998 Release */

undefined4 __cdecl __get_fname(int param_1)

{
  int iVar1;
  int *piVar2;
  
  iVar1 = 0;
  piVar2 = &DAT_00463320;
  do {
    if (*piVar2 == param_1) {
      return *(undefined4 *)(iVar1 * 8 + 0x463324);
    }
    piVar2 = piVar2 + 2;
    iVar1 = iVar1 + 1;
  } while (piVar2 < &DAT_004633f8);
  return 0;
}
