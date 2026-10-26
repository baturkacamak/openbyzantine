// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x43d883
// name: FUN_0043d883
// ranges: 0x43d883-0x43d93a; 0x43d940-0x43d97e
// calls: 0x43d883

undefined4 __thiscall FUN_0043d883(void *this,undefined4 param_1,undefined4 param_2)

{
  undefined4 local_44;
  undefined4 local_40;
  undefined4 local_3c;
  undefined4 local_38;
  undefined4 local_34;
  undefined4 local_30;
  undefined4 local_2c;
  undefined4 local_28;
  undefined4 local_24;
  undefined4 local_20;
  undefined4 local_1c;
  undefined4 local_18;
  undefined4 local_14;
  undefined4 local_10;
  undefined4 local_c;
  int local_8;
  
  local_44 = 0;
  local_40 = 0;
  local_3c = 0;
  local_38 = 0;
  local_34 = 0;
  local_30 = 0;
  local_2c = 0;
  local_28 = 0;
  local_24 = 0;
  local_20 = 0;
  local_1c = 0;
  local_18 = 0;
  local_14 = 0;
  local_10 = param_1;
  local_c = param_2;
  local_8 = ICSendMessage(*(undefined4 *)((int)this + 4),0x400f,&local_44,0x3c);
  if (local_8 == -2) {
    OutputDebugStringA(s_ICERR_BADFORMAT_0046190c);
  }
  else if (local_8 == -1) {
    OutputDebugStringA(s_ICERR_NOTSUPPORTED_00461920);
  }
  ICSendMessage(*(undefined4 *)((int)this + 4),0x4012,0,0);
  return 0;
}
