// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x40b368
// name: FUN_0040b368
// ranges: 0x40b368-0x40b444
// calls: 0x40b368

undefined4 __fastcall FUN_0040b368(int param_1)

{
  int iVar1;
  LONG LVar2;
  undefined4 uVar3;
  
  if (*(int *)(param_1 + 0x20) != 0) {
    while( true ) {
      LVar2 = InterlockedExchange((LONG *)(param_1 + 0x10),1);
      if (LVar2 == 0) break;
      Sleep(2);
    }
    iVar1 = *(int *)(param_1 + 0xc);
    InterlockedExchange((LONG *)(param_1 + 0x10),0);
    if (iVar1 != 0) {
      timeKillEvent(*(UINT *)(param_1 + 0xc));
      timeEndPeriod(0x21);
    }
    while( true ) {
      LVar2 = InterlockedExchange((LONG *)(param_1 + 0x10),1);
      if (LVar2 == 0) break;
      Sleep(2);
    }
    *(undefined4 *)(param_1 + 0xc) = 0;
    InterlockedExchange((LONG *)(param_1 + 0x10),0);
  }
  *(undefined4 *)(param_1 + 0x24) = 1;
  uVar3 = (**(code **)(**(int **)(param_1 + 4) + 0x48))(*(undefined4 *)(param_1 + 4));
  return uVar3;
}
