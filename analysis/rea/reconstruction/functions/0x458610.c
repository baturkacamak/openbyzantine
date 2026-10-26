// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x458610
// name: _signal
// ranges: 0x458610-0x45873f
// calls: -

/* Library Function - Single Match
    _signal
   
   Library: Visual Studio 1998 Release */

void __cdecl _signal(int param_1)

{
  int *piVar1;
  uint uVar2;
  BOOL BVar3;
  int in_stack_00000008;
  
  if (in_stack_00000008 == 4) {
    DAT_00462c74 = 0x16;
    return;
  }
  if (in_stack_00000008 == 3) {
    DAT_00462c74 = 0x16;
    return;
  }
  if (param_1 != 2) {
    if (((param_1 != 0x15) && (param_1 != 0x16)) && (param_1 != 0xf)) {
      if (((param_1 != 8) && (param_1 != 4)) && (param_1 != 0xb)) {
        DAT_00462c74 = 0x16;
        return;
      }
      uVar2 = siglookup(param_1);
      if (uVar2 == 0) {
        DAT_00462c74 = 0x16;
        return;
      }
      if (*(int *)(uVar2 + 4) != param_1) {
        return;
      }
      do {
        *(int *)(uVar2 + 8) = in_stack_00000008;
        piVar1 = (int *)(uVar2 + 0x10);
        uVar2 = uVar2 + 0xc;
      } while (*piVar1 == param_1);
      return;
    }
    if ((param_1 != 2) && (param_1 != 0x15)) goto LAB_004586ad;
  }
  if (DAT_0046401c == 0) {
    BVar3 = SetConsoleCtrlHandler((PHANDLER_ROUTINE)&LAB_00458770,1);
    if (BVar3 != 1) {
      DAT_00462c78 = GetLastError();
      DAT_00462c74 = 0x16;
      return;
    }
    DAT_0046401c = 1;
  }
LAB_004586ad:
  switch(param_1) {
  case 2:
    DAT_0046400c = in_stack_00000008;
    return;
  default:
    return;
  case 0xf:
    DAT_00464018 = in_stack_00000008;
    return;
  case 0x15:
    DAT_00464010 = in_stack_00000008;
    return;
  case 0x16:
    DAT_00464014 = in_stack_00000008;
    return;
  }
}
