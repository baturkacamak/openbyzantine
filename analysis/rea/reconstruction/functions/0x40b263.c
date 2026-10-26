// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x40b263
// name: FUN_0040b263
// ranges: 0x40b263-0x40b2d2; 0x40b2d8-0x40b367
// calls: 0x40b263, 0x40bb80

undefined4 __fastcall FUN_0040b263(DWORD_PTR param_1)

{
  LONG LVar1;
  MMRESULT MVar2;
  undefined4 local_8;
  
  if (*(int *)(param_1 + 0x20) != 0) {
    while (LVar1 = InterlockedExchange((LONG *)(param_1 + 0x10),1), LVar1 != 0) {
      Sleep(2);
    }
    *(undefined4 *)(param_1 + 0xc) = 0;
    InterlockedExchange((LONG *)(param_1 + 0x10),0);
    MVar2 = timeBeginPeriod(0x21);
    if (MVar2 != 0) {
      return 0xffffffff;
    }
    while (LVar1 = InterlockedExchange((LONG *)(param_1 + 0x10),1), LVar1 != 0) {
      Sleep(2);
    }
    MVar2 = timeSetEvent(0x21,10,FUN_0040bb80,param_1,1);
    *(MMRESULT *)(param_1 + 0xc) = MVar2;
    InterlockedExchange((LONG *)(param_1 + 0x10),0);
  }
  if ((*(int *)(param_1 + 8) != 0) && (*(int *)(param_1 + 4) != 0)) {
    local_8 = (**(code **)(**(int **)(param_1 + 4) + 0x30))(*(undefined4 *)(param_1 + 4),0,0,1);
  }
  return local_8;
}
