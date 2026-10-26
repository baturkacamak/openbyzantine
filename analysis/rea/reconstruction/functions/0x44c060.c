// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x44c060
// name: FUN_0044c060
// ranges: 0x44c060-0x44c0df
// calls: 0x44c060, 0x44c0e0

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __cdecl FUN_0044c060(UINT param_1,int param_2,int param_3)

{
  undefined4 *puVar1;
  
  _DAT_00462cb8 = 1;
  DAT_00462cb4 = (undefined1)param_3;
  if (param_2 == 0) {
    if ((DAT_004a8834 != (undefined4 *)0x0) &&
       (puVar1 = (undefined4 *)(DAT_004a8830 + -4), DAT_004a8834 <= puVar1)) {
      do {
        if ((code *)*puVar1 != (code *)0x0) {
          (*(code *)*puVar1)();
        }
        puVar1 = puVar1 + -1;
      } while (DAT_004a8834 <= puVar1);
    }
    FUN_0044c0e0((undefined4 *)&DAT_0045c020,(undefined4 *)&DAT_0045c02c);
  }
  FUN_0044c0e0((undefined4 *)&DAT_0045c030,(undefined4 *)&DAT_0045c034);
  if (param_3 == 0) {
                    /* WARNING: Subroutine does not return */
    ExitProcess(param_1);
  }
  return;
}
