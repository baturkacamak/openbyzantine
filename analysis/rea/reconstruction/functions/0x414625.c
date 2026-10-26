// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x414625
// name: FUN_00414625
// ranges: 0x414625-0x4146a6
// calls: 0x414625

undefined4 __thiscall FUN_00414625(void *this,int param_1)

{
  LONG LVar1;
  undefined4 uVar2;
  undefined4 local_c;
  
  if (param_1 == 0) {
    local_c = 0;
  }
  else {
    local_c = 1;
  }
  while( true ) {
    LVar1 = InterlockedExchange((LONG *)&DAT_00486798,1);
    if (LVar1 == 0) break;
    Sleep(2);
  }
  uVar2 = (**(code **)(**(int **)((int)this + 4) + 0x2c))(*(undefined4 *)((int)this + 4),0,local_c);
  InterlockedExchange((LONG *)&DAT_00486798,0);
  return uVar2;
}
